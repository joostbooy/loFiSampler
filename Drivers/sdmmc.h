#ifndef Sdmmc_h
#define Sdmmc_h

#include "stm32f7xx.h"
#include "micros.h"

// Based on:
// https://github.com/westlicht/performer/blob/master/src/platform/stm32/drivers/SdCard.cpp

class Sdmmc {

public:

	enum Command {
		CMD0_SOFT_RESET					= 0,
		CMD1_INITIALISE					= 1,
		CMD8_SEND_IF_COND				= 8,	// Interface condition
		CMD9_READ_CSD_REGISTER			= 9,	// Card specific data
		CMD10_READ_CID_REGISTER			= 10,	// Card id
		CMD12_STOP_READ					= 12,
		ACMD13_SD_STATUS				= 13,
		CMD16_CHANGE_BLOCK_SIZE			= 16,
		CMD17_READ_BLOCK				= 17,
		CMD18_READ_MULTIPLE_BLOCKS		= 18,
		ACMD23_NUM_OF_BLOCKS			= 23,
		CMD24_WRITE_BLOCK				= 24,
		CMD25_WRITE_MULTIPLE_BLOCKS		= 25,
		CMD32_ERASE_WR_BLK_START_ADDR	= 32,
		CMD33_ERASE_WR_BLK_END_ADDR		= 33,
		CMD38_ERASE						= 38,
		ACMD41_INIT_SDC					= 41,
		CMD55_APP_COMMAND 				= 55,	// Precedes all app commands
		CMD58_READ_OCR					= 58	// Operation condition
	};

	enum Error {
		Success,
		InProgress,
		Timeout,
		CRCFail,
		Unknown,
	};

	void init();

	bool status() {
		if (initialised_) {
			initialised_ = waitDataReady();
		}

		if (!initialised_) {
			initialised_ = init_card();
		}

		return initialised_;
	}

	bool read(uint8_t* buf, uint32_t sector, uint8_t count) {
		uint8_t *data = buf;

		for (uint32_t i = 0; i < count; ++i) {
			if (!readBlocks(sector + i, data)) {
				return false;
			}
			data += 512;
		}
		return true;
	}

	bool write(const uint8_t* buf, uint32_t sector, uint8_t count) {
		const uint8_t *data = buf;

		for (uint32_t i = 0; i < count; ++i) {
			if (!writeBlocks(sector + i, data)) {
				return false;
			}
			data += 512;
		}
		return true;
	}

	bool initialised() {
		return initialised_;
	}

	uint32_t sector_count() {
		return cardInfo.size;
	}

	uint32_t sector_size() {
		return 512;
	}

	volatile bool dma_busy() {
		return dma_busy_;
	}

	void unlock_dma() {
		SDMMC2->DCTRL &= ~SDMMC_DCTRL_DMAEN;
		DMA2_Stream0->CR &= ~DMA_SxCR_EN;
		dma_busy_ = false;
	}

	void lock_dma() {
		dma_busy_ = true;
	}

	static Sdmmc *sdmmc_;


	const char *flags_text() {
		if (flags_ & SDMMC_STA_DCRCFAIL) {
			return "SDMMC_STA_DCRCFAIL";
		}

		if (flags_ & SDMMC_STA_TXUNDERR) {
			return "SDMMC_STA_TXUNDERR";
		}

		if (flags_ & SDMMC_STA_DTIMEOUT) {
			return "SDMMC_STA_DTIMEOUT";
		}

		if (flags_ & SDMMC_STA_DBCKEND) {
			return "SDMMC_STA_DBCKEND";
		}

		if (flags_ & SDMMC_STA_DATAEND) {
			return "SDMMC_STA_DATAEND";
		}

		return "UNKOWN";
	}


private:
	volatile uint32_t flags_ = 0;

	enum DmaType {
		MEM_TO_SD,
		SD_TO_MEM
	};

	struct CardInfo {
		uint16_t rca = 0;	// Relative Card Address, used when sending certain commands.
		bool ccs = false;	// If ccs true then card is SDHC/SDXC with block adressing, else SDSC with byte adressing
		uint32_t size = 0;	// Size is in 512 byte blocks. Max size is 2T or 0x100000000.
	};

	CardInfo cardInfo;
	bool initialised_ = false;
	volatile bool dma_busy_ = false;

	void init_dma(uint32_t buffer, DmaType type);

	void powerOff() {
		//if (initialised_ == false) {
		SDMMC2->POWER = 0;
		//	while (SDMMC2->POWER);
		//} else {
		//	clock_resume();
		//}
	}

	//	void clock_resume() {
	//		SDMMC2->CLKCR = SDMMC_CLKCR_CLKEN | (1 << 11) | 0x00;		//24 mHz, 4 bit
	//0x76 // SDMMC2 clock 400kHz
	//0x2e // SDMMC2 clock 1MHz
	//0x16 // SDMMC2 clock 2MHz
	//0x0a // SDMMC2 clock 4MHz
	//0x05 // SDMMC2 clock 6.85MHz
	//0x04 // SDMMC2 clock 8MHz
	//0x03 // SDMMC2 clock 9.6MHz
	//0x02 // SDMMC2 clock 12MHz
	//0x01 // SDMMC2 clock 16MHz
	//0x00 // SDMMC2 clock 24MHz
	//	}

	bool init_card() {
		// Power down and start in 400 kHz, 1 bit mode
		//SDMMC2->POWER = 0;
		//while (SDMMC2->POWER);

		SDMMC2->POWER = SDMMC_POWER_PWRCTRL;
		while (SDMMC2->POWER != SDMMC_POWER_PWRCTRL);
		SDMMC2->CLKCR = SDMMC_CLKCR_CLKEN | 118;

		if (sendCommandRetry(0, 0) != Success) {
			powerOff();
			return false;
		}

		Error result;

		bool hcs = false;
		result = sendCommandRetry(8, 0x1F1);
		if ((result == Success) && (SDMMC2->RESP1 == 0x1F1)) {
			hcs = true;
		} else if (result == Timeout) {
			hcs = false;
		} else {
			powerOff();
			return false;
		}

		const uint32_t OCR_BUSY = 0x80000000;
		const uint32_t OCR_HCS = 0x40000000;
		const uint32_t OCR_CCS = 0x40000000;

		bool acmd41_success = false;


		uint32_t start = Micros::read();

		while ((Micros::read() - start) < 2000000UL) {
			result = sendAppCommand(41, 0x100000 | (hcs ? OCR_HCS : 0));
			uint32_t response = SDMMC2->RESP1;
			if (result == CRCFail && (response & OCR_BUSY) != 0) {
				cardInfo.ccs = (response & OCR_CCS) != 0;
				acmd41_success = true;
				break;
			}
		}

		if (!acmd41_success) {
			powerOff();
			return false;
		}

		if (sendCommandRetry(2, 0) != Success) {
			powerOff();
			return false;
		}

		bool cmd3_success = false;
		if (sendCommandRetry(3, 0) == Success) {
			uint32_t response = SDMMC2->RESP1;
			cardInfo.rca = response >> 16;
			if (cardInfo.rca != 0) {
				cmd3_success = true;
			}
		}

		if (!cmd3_success) {
			powerOff();
			return false;
		}

		if (sendCommandRetry(9, cardInfo.rca << 16) != Success) {
			powerOff();
			return false;
		}

		uint32_t csd_version = SDMMC2->RESP1 >> 30;

		if (csd_version == 0) {
			// Until I find an old card, this is untested.
			uint32_t read_bl_len = (SDMMC2->RESP2 >> 16) & 0xF;
			uint32_t c_size = ((SDMMC2->RESP2 & 0x3FF) << 2) | (SDMMC2->RESP3 >> 30);
			uint32_t c_size_mult = (SDMMC2->RESP3 >> 15) & 0x7;
			uint32_t mult = 1 << (c_size_mult + 2);
			uint32_t blocknr = (c_size + 1) * mult;
			uint32_t block_len = 1 << read_bl_len;
			cardInfo.size = (block_len * blocknr) >> 9;
		} else if (csd_version == 1) {
			uint32_t c_size = ((SDMMC2->RESP2 & 0x3F) << 16) | (SDMMC2->RESP3 >> 16);
			cardInfo.size = (c_size + 1) << 10;
		} else {
			powerOff();
			return false;
		}

		// Select the card
		if (sendCommandRetry(7, cardInfo.rca << 16) != Success) {
			powerOff();
			return false;
		}

		// Enable 4 bit bus mode
		if (sendAppCommand(6, 2) != Success) {
			powerOff();
			return false;
		}

		//start clock 24 mHz, 4 bit
		SDMMC2->CLKCR = SDMMC_CLKCR_CLKEN | (1 << 11) | 0x00; //0x02

		return true;
	}

	bool waitDataReady() {
		uint32_t start = Micros::read();

		while ((Micros::read() - start) < 1000000UL) {
			if (sendCommandWait(13, cardInfo.rca << 16) == Success && (SDMMC2->RESP1 & 0x100) != 0) {
				return true;
			}
		}
		return false;
	}

	void sendCommand(uint32_t cmd, uint32_t arg) {
		cmd &= 0x3F;	// SDMMC_CMD_CMDINDEX_MASK

		uint32_t waitresp = SDMMC_RESPONSE_SHORT;
		if (cmd == 0) {
			waitresp = SDMMC_RESPONSE_NO;
		} else if (cmd == 2 || cmd == 9 || cmd == 10) {
			waitresp = SDMMC_RESPONSE_LONG;
		}

		SDMMC2->ICR = 0x7FF; 								// Reset all signals we use (and some we don't).
		SDMMC2->ARG = arg; 								// The arg must be set before the command.
		SDMMC2->CMD = (cmd | SDMMC_CMD_CPSMEN | waitresp);	// Set the command and associated bits.
	}

	Error sendAppCommand(uint32_t cmd, uint32_t arg, int maxRetries = 5) {
		Error result = Unknown;
		Error expected = (cmd == 41) ? CRCFail : Success;
		for (int i = 0; i < maxRetries; ++i) {
			result = sendCommandWait(55, cardInfo.rca << 16);
			if (result != Success) {
				continue;
			}
			result = sendCommandWait(cmd, arg);
			if (result == expected) {
				break;
			}
		}
		return result;

	}

	Error sendCommandWait(uint32_t cmd, uint32_t arg) {
		Error result;
		sendCommand(cmd, arg);
		while ((result = commandResult()) == InProgress);
		return result;
	}

	Error sendCommandRetry(uint32_t cmd, uint32_t arg, int maxRetries = 5) {
		Error result = Unknown;
		for (int i = 0; i < maxRetries; ++i) {
			result = sendCommandWait(cmd, arg);
			if (result == Success) {
				break;
			}
		}
		return result;
	}

	Error commandResult() {
		uint32_t status = SDMMC2->STA & 0xFFF;

		if (status & SDMMC_STA_CMDACT) {
			return InProgress;
		}

		if (status & SDMMC_STA_CMDREND) {
			SDMMC2->ICR = SDMMC_STA_CMDREND;
			return Success;
		}

		if (status & SDMMC_STA_CMDSENT) {
			SDMMC2->ICR = SDMMC_STA_CMDSENT;
			return Success;
		}

		if (status & SDMMC_STA_CTIMEOUT) {
			SDMMC2->ICR = SDMMC_STA_CTIMEOUT;
			return Timeout;
		}

		if (status & SDMMC_STA_CCRCFAIL) {
			SDMMC2->ICR = SDMMC_STA_CCRCFAIL;
			return CRCFail;
		}

		return Unknown;
	}

	bool readBlocks(uint32_t address, void* buffer, uint16_t count = 1) {
		if (!waitDataReady()) {
			return false;
		}

		if (!cardInfo.ccs) {
			address *= 512;
			if (sendCommandRetry(16, 512) != Success) {
				return false;
			}
		}

		SDMMC2->DCTRL = 0;

		init_dma((uint32_t)buffer, SD_TO_MEM);

		SDMMC2->DTIMER = 2400000;		//100ms (on 24Mhz bus clock)
		SDMMC2->DLEN = 512 * count;
		SDMMC2->DCTRL = (9 << 4) | SDMMC_DCTRL_DTDIR | SDMMC_DCTRL_DTEN | SDMMC_DCTRL_DMAEN;

		uint32_t cmd = count > 1 ? 18 : 17;
		if (sendCommandWait(cmd, address) != Success) {
			return false;
		}

		while (dma_busy_) {};

		//stop transfer if needed
		if (count > 1) {
			if (sendCommandWait(12, 0) != Success) {
				return false;
			}
		}

		// check errors
		const uint32_t succes_flags = SDMMC_STA_DBCKEND | SDMMC_STA_DATAEND;
		const uint32_t error_flags = SDMMC_STA_DCRCFAIL | SDMMC_STA_RXOVERR | SDMMC_STA_DTIMEOUT; //SDMMC_STA_STBITERR

		while (true) {
			volatile uint32_t flags = SDMMC2->STA;
			if (flags & (succes_flags | error_flags)) {
				if (flags & error_flags) {
					return false;
				} else if (flags & succes_flags) {
					break;
				}
			}
		}

		return true;
	}

	bool writeBlocks(uint32_t address, const void* buffer, uint16_t count = 1) {
		if (!waitDataReady()) {
			return false;
		}

		if (!cardInfo.ccs) {
			address *= 512;
			if (sendCommandRetry(16, 512) != Success) {
				return false;
			}
		}

		uint32_t cmd = count > 1 ? 25 : 24;
		if (sendCommandWait(cmd, address) != Success) {
			return false;
		}

		SDMMC2->DCTRL = 0;

		init_dma((uint32_t)buffer, MEM_TO_SD);

		SDMMC2->DTIMER = 12000000;	// 500ms (on 24Mhz bus clock)  24000000
		SDMMC2->DLEN = 512 * count;
		SDMMC2->DCTRL = (9 << 4) | SDMMC_DCTRL_DTEN | SDMMC_DCTRL_DMAEN;

		while (dma_busy_) {};

		//stop transfer if needed
		if (count > 1) {
			if (sendCommandWait(12, 0) != Success) {
				return false;
			}
		}

		// check errors
		const uint32_t succes_flags = SDMMC_STA_DBCKEND | SDMMC_STA_DATAEND; //SDMMC_STA_TXACT
		const uint32_t error_flags = SDMMC_STA_DCRCFAIL | SDMMC_STA_TXUNDERR | SDMMC_STA_DTIMEOUT; //SDMMC_STA_STBITERR

		while (true) {
			volatile uint32_t flags = SDMMC2->STA;

			flags_ = flags;

			if (flags & (succes_flags | error_flags)) {
				if (flags & error_flags) {
					return false;
				} else if (flags & succes_flags) {
					break;
				}
			}
		}
		return true;
	}
};

#endif
