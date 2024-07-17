#ifndef DelayEngine_h
#define DelayEngine_h

#include "delay.h"

class DelayEngine {

public:

	void init(Delay *delay) {
		delay_ = delay;
		pos_ = 0;
		std::fill(delay_line_[0], delay_line_[delay_line_size_], 0);
	}

	void process(int16_t *buffer, size_t size) {
		while (size--) {
			int16_t input = *buffer;
			delay_line_[pos_] = input + (delay_line_[pos_] * delay_->feedback());

			*buffer++ = Dsp::cross_fade(input, delay_line_[pos_], delay_->mix());

			if (++pos_ >= (delay_->amount() * delay_line_size_)) {
				pos_ = 0;
			}
		}
	}

private:
	Delay *delay_;
	size_t pos_;
	size_t delay_line_size_ = SAMPLE_RATE; // 1 second
	int16_t *delay_line_;
};

#endif
