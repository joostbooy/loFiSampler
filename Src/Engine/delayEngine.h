#ifndef DelayEngine_h
#define DelayEngine_h

#include "settings.h"
#include <algorithm>

class DelayEngine {

public:

	static const size_t kDelayLineSize = SAMPLE_RATE * 3;
	static const size_t kRamSize = kDelayLineSize * 2;

	void init(Settings *settings) {
		pos_ = 0;
		delay_ = &settings->delay();
		std::fill(&delay_l_[0], &delay_l_[kDelayLineSize], 0);
		std::fill(&delay_r_[0], &delay_r_[kDelayLineSize], 0);
	}

	void process(int16_t *left, int16_t *right, size_t size, size_t inc) {
		while (size--) {
			int16_t l = *left;
			int16_t r = *right;

			delay_l_[pos_] = l + (delay_l_[pos_] * delay_->feedback());
			delay_r_[pos_] = r + (delay_r_[pos_] * delay_->feedback());

			*left = Dsp::cross_fade(l, delay_l_[pos_], delay_->mix());
			*right = Dsp::cross_fade(r, delay_r_[pos_], delay_->mix());

			left += inc;
			right += inc;

			if (++pos_ >= (delay_->amount() * kDelayLineSize)) {
				pos_ = 0;
			}
		}
	}

private:
	Delay *delay_;
	size_t pos_;
	int16_t *delay_l_;
	int16_t *delay_r_;
};

#endif
