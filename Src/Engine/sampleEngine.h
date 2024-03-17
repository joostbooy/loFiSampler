#ifndef SampleEngine_h
#define SampleEngine_h

#include "sample.h"
#include "modulationEngine.h"

class SampleEngine {

public:

	enum State {
		BACKWARD 	= -1,
		IDLE		= 0,
		FORWARD		= 1
	};

	void init() {
		state_ = IDLE;
	}

	void note_on(MidiEngine::Event &e, Sample *sample, Instrument *instrument) {
		sample_ = sample;
		instrument_ = instrument;
		key_pressed_ = true;

		note_ = e.data[0];
		velocity_ = (1.f / 127.f) * e.data[1];

		if (sample_->play_mode() == Sample::FORWARD) {
			phase_ = sample_->start();
			state_ = FORWARD;
		} else {
			phase_ = sample_->end();
			state_ = BACKWARD;
		}
	}

	void note_off() {
		key_pressed_ = false;
	}

	void fill(int16_t *buffer, ModulationEngine::Frame *frame, const size_t size) {
		for (int i = 0; i < size; ++i) {
			*buffer++ = next(*frame++);
		}
	}

	int16_t next(ModulationEngine::Frame *frame) {
		if (state_ == IDLE) {
			return 0;
		}

		uint32_t intergral = static_cast<uint32_t>(phase_);
		float fractional = phase_ - intergral;

		uint8_t shifts = instrument_->bit_shifts();
		int16_t a = sample_->data(phase_intergral);
		int16_t b = sample_->data(phase_intergral + direction_);
		int16_t value = (Dsp::cross_fade(a, b, fractional) >> shifts) << shifts;

		phase_ += get_inc(note_, sample_->root_note(), instrument_->bend_range(), frame->bend);

		if (state_ == FORWARD) {
			if (is_looping()) {
				next_loop_forward(intergral);
			} else {
				next_forward(intergral);
			}
		} else if (state_ == BACKWARD) {
			if (is_looping()) {
				next_loop_backward(intergral);
			} else {
				next_backward(intergral);
			}
		}

		return value * velocity_ * frame->gain;
	}

private:
	float phase_;
	float velocity_;
	bool key_pressed_;
	uint8_t note_;
	State state_;
	Sample_ *sample_;
	Instrument *instrument_;

	inline bool is_looping() {
		return sample_->loop() && key_pressed_;
	}

	inline void next_forward((uint32_t intergral) {
		if (intergral >= sample_->end()) {
			if (sample_->u_turn() && (sample_->play_mode() == Sample::FORWARD)) {
				state_ = BACKWARD;
			} else {
				state_ = IDLE;
			}
		}
	}

	inline void next_backward((uint32_t intergral) {
		if (intergral <= sample_->start()) {
			if (sample_->u_turn() && (sample_->play_mode() == Sample::BACWARD)) {
				state_ = FORWARD;
			} else {
				state_ = IDLE;
			}
		}
	}

	inline void next_loop_forward(uint32_t intergral) {
		if (intergral >= sample_->loop_end()) {
			if (sample_->u_turn()) {
				state_ = BACKWARD;
			} else {
				phase_ = sample_->loop_start();
			}
		}
	}

	inline void next_loop_backward(uint32_t intergral) {
		if (intergral <= sample_->loop_start()) {
			if (sample_->u_turn()) {
				state_ = FORWARD;
			} else {
				phase_ = sample_->loop_end();
			}
		}
	}

	inline float get_inc(int note, int root_note, int bend_range, float bend = 0.5f) {
		int index = 128 - (note - root_note);
		float a = lut_frequency_ratio[stmlib::clip_max(255, index + bend_range)];
		float b = lut_frequency_ratio[index];
		float c = lut_frequency_ratio[stmlib::clip_min(0, index - bend_range)];
		return Dsp::cross_fade(a, b, c, bend) * state_;
	}
};

#endif
