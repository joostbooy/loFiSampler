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

	float next() {
		phase_ += lfo_->inc();
		if (phase_ >= 1.f) {
			phase_ = 0.f;
		}

		float tri_ = triangle(phase_, lfo_->skew_float());
		return Curve::read(tri_, lfo_->shape_float());
	}

private:
	Lfo *lfo_;
	float inc_;
	float phase_;

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
