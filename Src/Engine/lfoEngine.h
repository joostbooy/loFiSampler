#ifndef LfoEngine_h
#define LfoEngine_h

#include "lfo.h"
#include "curve.h"

class LfoEngine {

public:

	void init(Lfo *lfo) {
		lfo_ = lfo;
		inc_ = 0.f;
		phase_ = 0.f;
		reset();
	}

	void reset() {
		phase_ = 0.f;
	}

	float next_sample() {
		phase_ += lfo_->inc();
		if (phase_ >= 1.f) {
			phase_ = 0.f;
		}
		return Curve::read(phase_, lfo_->shape());
		//return Dsp::cross_fade(a, b, triangle(phase_));
	}

private:
	float phase_;
	float inc_;
	Lfo *lfo_;

	inline float triangle(float phase) {
		return phase < 0.5f ? phase * 2.f : (1.f - phase) * 2.f;
	}

	inline float triangle(float phase, float skew) {
		if (phase < skew) {
			float inc_up = 1.0f / skew;
			return phase * inc_up;
		} else {
			float inc_down = 1.0f / (1.0f - skew);
			return 1.0f - (phase - skew) * inc_down;
		}
	}
};

#endif
