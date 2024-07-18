#ifndef Limiter_h
#define Limiter_h

// Pinchenettes
#include <algorithm>

class Limiter {
public:

	inline void init() {
		std::fill(&peak_[0], &peak_[Dac::kNumChannels], 0.5f);
	}

	inline void process(Dac::Buffer *buffer, size_t size) {
		for (size_t chn = 0; chn < Dac::kNumChannels; ++chn) {
			int16_t *data = &buffer[0].channel[chn];

			for (size_t i = 0; i < size; ++i) {
				float s = int_to_float(*data);
				float error = fabsf(s) - peak_[chn];
				peak_[chn] += (error > 0 ? 0.05f : 0.00002f) * error;
				float gain = (peak_[chn] <= 1.0f ? 1.0f : 1.0f / peak_[chn]);
				*data = float_to_int(s * gain * 0.8f);
				data += Dac::kNumChannels;
			}
		}
	}

private:
	float peak_[Dac::kNumChannels];

	inline float int_to_float(int value) {
		return value * (1.f * 32767.f);
	}

	inline int float_to_int(float value) {
		int value_ = value * 32767;
		return value_ > 32767 ? 32767 : (value_ < -32768 ? -32768 : value_);
	}
};

#endif
