#ifndef Dac_h
#define Dac_h

#include "stm32f7xx.h"
#include "micros.h"
#include "debug.h"
#include "lookupTables.h"

class Dac {

public:
	static Dac *dac_;

	static const size_t kNumChannels = 8;
	static const size_t kBlockSize = 8;
	static const size_t kUpdateRate = SAMPLE_RATE / kBlockSize / kNumChannels;

	struct Buffer {
		int16_t channel[kNumChannels];
	}buffer_[kBlockSize];

	void init();
	void start(void(*callback)(Buffer*, const size_t));

	void fill(const size_t offset) {
		callback_(buffer_, kBlockSize);

		uint16_t *ptr = reinterpret_cast<uint16_t*>(&dma_buffer_[offset * (kDmaBufferSize / 2)]);

		for (size_t i = 0; i < kBlockSize; ++i) {
			for (size_t chn = 0; chn < kNumChannels; ++chn) {
				uint16_t sample = buffer_[i].channel[chn] + 32768;
				*ptr++ = 0x1000 | (chn << 9) | (sample >> 8);
				*ptr++ = sample << 8;
			}
		}
	}

	// TEMP !
	void write(int channel, int value) {
		spi_write(3, channel, value, 15);
	}

private:
	void(*callback_)(Buffer*, const size_t);

	static const uint32_t kDmaBufferSize = kBlockSize * kNumChannels * 2;
	uint32_t dma_buffer_[kDmaBufferSize];

	void spi_write(uint8_t data);
	void spi_write(uint8_t command, uint8_t address, uint16_t data, uint8_t function);
};

#endif
