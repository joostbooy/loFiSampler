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
	void fill(const size_t offset);

private:
	void(*callback_)(Channel*, const size_t);
	static const uint32_t kDmaBufferSize = BLOCK_SIZE * (kNumStereoChannels * 2) * 4 * 2; // x4 words x2 size

	uint16_t dma_buffer_[kDmaBufferSize];

	static bool first_frame_;
	static uint16_t left_channel_bits_[kNumStereoChannels];
	static uint16_t right_channel_bits_[kNumStereoChannels];
};

#endif
