#ifndef DelayEngine_h
#define DelayEngine_h

#include "settings.h"
#include <algorithm>

class DelayEngine {

public:

	void init(Settings *settings) {
		pos_ = 0;
		delay_ = &settings->delay();
		std::fill(&delay_l_[0], &delay_l_[Delay::kMaxDelay], 0);
		std::fill(&delay_r_[0], &delay_r_[Delay::kMaxDelay], 0);
	}

	void process(Dac::Buffer *buffer, size_t size) {
		int channel = delay_->channel() * 2;
		int16_t *left = &buffer[0].channel[channel];
		int16_t *right = &buffer[0].channel[channel + 1];

		while (size--) {
			int16_t l = *left;
			int16_t r = *right;

			delay_l_[pos_] = l + (delay_l_[pos_] * delay_->feedback());
			delay_r_[pos_] = r + (delay_r_[pos_] * delay_->feedback());

			*left = Dsp::cross_fade(l, delay_l_[pos_], delay_->mix());
			*right = Dsp::cross_fade(r, delay_r_[pos_], delay_->mix());

			left += Dac::kNumChannels;
			right += Dac::kNumChannels;

			if (++pos_ >= delay_->num_samples()) {
				pos_ = 0;
			}
		}
	}

private:
	Delay *delay_;
	size_t pos_;
	int16_t delay_l_[Delay::kMaxDelay];
	int16_t delay_r_[Delay::kMaxDelay];
};

#endif
