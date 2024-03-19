#ifndef LfoEngine_h
#define LfoEngine_h

#include "lfo.h"
#include "curve.h"
#include "rng.h"

class LfoEngine {

public:

	enum Stage {
		FALLING = 0,
		RISING = 1,
	};

	void init(Lfo *lfo) {
		lfo_ = lfo;
		reset();
	}

	void reset() {
		phase_ = 0.f;
		set_stage(RISING);
	}

	float phase() {
		return phase_;
	}

	float next() {
		phase_ += lfo_->inc();
		if (phase_ >= 1.f) {
			phase_ = 0.f;
		}

		float skew_phase_;
		float skew = lfo_->skew_float();

		if (phase_ < skew) {
			set_stage(RISING);
			skew_phase_ = phase_ * (1.0f / skew);
		} else {
			set_stage(FALLING);
			skew_phase_ = (phase_ - skew) * (1.0f / (1.0f - skew));
		}

		float curve_phase = Curve::read(skew_phase_, lfo_->shape_float());
		value_ = Dsp::cross_fade(last_value_, target_value_, curve_phase);

		return value_;
	}

private:
	Lfo *lfo_;
	Stage stage_;
	float phase_;
	float value_;
	float last_value_;
	float target_value_;

	inline void set_stage(Stage stage) {
		if (stage_ != stage) {
			stage_ = stage;
			last_value_ = value_;

			if (lfo_->randomise()) {
				target_value_ = Rng::reciprocal();
			} else {
				target_value_ = static_cast<float>(stage);
			}
		}
	}

};

#endif
