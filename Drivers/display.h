#ifndef Display_h
#define Display_h

#include "stm32f7xx.h"
#include "debug.h"
#include "micros.h"

class Display {

public:

	void init();

	static Display *display_;

	inline void select(){
		GPIOC->BSRR = GPIO_PIN_14 << 16;
	}

	inline void deselect(){
		GPIOC->BSRR = GPIO_PIN_14;
	}

	inline void cd_high(){
		GPIOC->BSRR = GPIO_PIN_15;
	}

	inline void cd_low(){
		GPIOC->BSRR = GPIO_PIN_15 << 16;
	}

	void sendCommand(uint8_t cmd) {
		cd_low();
		spi_write(cmd);
	}

	void sendData(uint8_t data) {
		cd_high();
		spi_write(data);
	}

	void sendCommand(uint8_t cmd, uint8_t data) {
		sendCommand(cmd);
		sendData(data);
	}

	void sendCommand(uint8_t cmd, uint8_t data_a, uint8_t data_b) {
		sendCommand(cmd);
		sendData(data_a);
		sendData(data_b);
	}

	void set_col_address(uint8_t a, uint8_t b) {
		sendCommand(0x15, a, b);
	}

	void set_row_address(uint8_t a, uint8_t b) {
		sendCommand(0x75, a, b);
	}

	void enable_write() {
		sendCommand(0x5C);
	}

	void unlock_dma() {
		SPI5->CR2 &= ~SPI_CR2_TXDMAEN;
		DMA2_Stream4->CR &= ~DMA_SxCR_EN;
		dma_busy_ = false;
	}

	volatile bool dma_busy() {
		return dma_busy_;
	}

	void sendBuffer(uint8_t* data, uint32_t size) {
		while (dma_busy_) {};
		dummy = SPI5->DR;

		set_col_address(0x1C, 0x5B);
		set_row_address(0x00, 0x3F);
		enable_write();

		cd_high();

		DMA2->HIFCR |= DMA_HIFCR_CTCIF4 | DMA_HIFCR_CHTIF4 | DMA_HIFCR_CTEIF4 | DMA_HIFCR_CDMEIF4 | DMA_HIFCR_CFEIF4;
		DMA2_Stream4->PAR = reinterpret_cast<uint32_t>(&SPI5->DR);
		DMA2_Stream4->M0AR = reinterpret_cast<uint32_t>(&data[0]);
		DMA2_Stream4->NDTR = size;
		DMA2_Stream4->CR |= DMA_SxCR_EN;
		SPI5->CR2 |= SPI_CR2_TXDMAEN;

		dma_busy_ = true;
	}

private:
	volatile uint8_t dummy;
	volatile bool dma_busy_;

	void spi_write(uint8_t data) {
		while (!(SPI5->SR & SPI_FLAG_TXE));
		SPI5->DR = data;

		while (!(SPI5->SR & SPI_FLAG_RXNE));
		dummy = SPI5->DR;
	}
};

#endif
