#ifndef Dac_h
#define Dac_h

#include "stm32f7xx.h"
#include "micros.h"
#include "lookupTables.h"

class Dac {

public:
	static Dac *dac_;
	static const size_t kNumStereoChannels = 4;
	static const size_t kUpdateRate = SAMPLE_RATE / BLOCK_SIZE / (kNumStereoChannels * 2);

	struct Channel {
		int16_t left[BLOCK_SIZE];
		int16_t right[BLOCK_SIZE];
	}channel_[kNumStereoChannels];

	void init();
	void start(void(*callback)(Channel*, const size_t));

	void fill(const size_t offset) {
		callback_(channel_, BLOCK_SIZE);

		uint16_t *ptr = reinterpret_cast<uint16_t*>(&dma_buffer_[offset * (kDmaBufferSize / 2)]);

		for (size_t i = 0; i < BLOCK_SIZE; ++i) {
			for (size_t chn = 0; chn < kNumStereoChannels; ++chn) {
				uint16_t left = channel_[chn].left[i] + 32768;
				uint16_t right = channel_[chn].right[i] + 32768;
				*ptr++ = 0x1000 | ((chn * 2) << 9) | (left >> 8);
				*ptr++ = left << 8;
				*ptr++ = 0x1000 | (((chn * 2) + 1) << 9) | (right >> 8);
				*ptr++ = right << 8;
			}
		}
	}

	// TEMP !
	void write(int channel, int value) {
		spi_write(3, channel, value, 15);
	}

private:
	void(*callback_)(Channel*, const size_t);
	static const uint32_t kDmaBufferSize = BLOCK_SIZE * (kNumStereoChannels * 2) * 2;
	uint32_t dma_buffer_[kDmaBufferSize];

	void spi_write(uint8_t data);
	void spi_write(uint8_t command, uint8_t address, uint16_t data, uint8_t function);
};

#endif
