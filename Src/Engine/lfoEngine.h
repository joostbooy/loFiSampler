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

	void retrigger(uint8_t port, uint8_t channel) {
		if (lfo_->retrigger_port_accepted(port) && lfo_->retrigger_channel_accepted(channel)) {
			reset();
		}
	}

	float phase() { return phase_; }
	float value() { return value_; }

	float next() {
		phase_ += lfo_->inc();
		if (phase_ >= 1.f) {
			phase_ = 0.f;
		}

		float skew_phase;
		float skew_amount = lfo_->skew();

		if (phase_ < skew_amount) {
			set_stage(RISING);
			skew_phase = phase_ * (1.0f / skew_amount);
		} else {
			set_stage(FALLING);
			skew_phase = (phase_ - skew_amount) * (1.0f / (1.0f - skew_amount));
		}

		float curve_shape = (stage_ == RISING) ? lfo_->shape() : 1.f - lfo_->shape();
		float curve_phase = Curve::read(skew_phase, curve_shape);
		value_ = Dsp::cross_fade(last_value_, target_value_, curve_phase);

		return value_;
	}

private:
	Lfo *lfo_;
	Stage stage_ = RISING;
	float phase_ = 0.f;
	float value_ = 0.f;
	float last_value_ = 0.f;
	float target_value_ = 0.f;

	inline void set_stage(Stage stage) {
		if (stage_ != stage) {
			stage_ = stage;

			last_value_ = value_;

			if (lfo_->randomise()) {
				target_value_ = Rng::reciprocal();
			} else {
				target_value_ = (stage_ == RISING) ? 1.f : 0.f;
			}
		}
	}

};

#endif
