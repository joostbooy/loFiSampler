#ifndef SampleEngine_h
#define SampleEngine_h

#include "sample.h"
#include "sampleQue.h"
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

	State state() {
		return state_;
	}

	uint32_t phase() {
		return phase_;
	}

	void note_on(SampleQue::Event &e) {
		sample_ = e.sample_;
		instrument_ = e.instrument_;
		note_ = e.midi_event_.data[0];

		key_pressed_ = true;

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
		// copy settings;
		//sample_ = sample;

		for (size_t i = 0; i < size; ++i) {
			*buffer++ = next(frame++);
		}
	}

	int16_t next(ModulationEngine::Frame *frame) {
		// apply_modulation(frame);

		uint32_t intergral = static_cast<uint32_t>(phase_);
		float fractional = phase_ - intergral;

		int16_t a = sample_->data(intergral);
		int16_t b = sample_->data(intergral + state_);
		uint8_t shifts = instrument_->bit_shifts();
		int16_t value = (Dsp::cross_fade(a, b, fractional) >> shifts) << shifts;

		phase_ += get_inc(note_, sample_->root_note(), instrument_->bend_range(), sample_->cents(), frame->data[Modulation::BEND]);

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

		return value * sample_->gain() * instrument_->gain();
	}

private:
	float phase_;
	bool key_pressed_;
	uint8_t note_;
	State state_;
	Sample *sample_;
	Instrument *instrument_;

	inline bool is_looping() {
		return sample_->loop() && key_pressed_;
	}

	inline void next_forward(uint32_t intergral) {
		if (intergral >= sample_->end()) {
			if (sample_->u_turn() && (sample_->play_mode() == Sample::FORWARD)) {
				state_ = BACKWARD;
			} else {
				state_ = IDLE;
			}
		}
	}

	inline void next_backward(uint32_t intergral) {
		if (intergral <= sample_->start()) {
			if (sample_->u_turn() && (sample_->play_mode() == Sample::BACKWARD)) {
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

	inline float get_inc(int note, int root_note, int bend_range, float cents, float bend) {
		int semitone = 128 - (note - root_note);
		float a = lut_semitone_ratio[stmlib::clip_min(0, semitone - bend_range)];
		float b = lut_semitone_ratio[semitone];
		float c = lut_semitone_ratio[stmlib::clip_max(255, semitone + bend_range)];
		return Dsp::cross_fade(a, b, c, bend) * state_;
	}

	void apply_modulation(ModulationEngine::Frame *frame) {
		if (instrument_->modulation()) {
			//	sample_.set_gain(sample_src_->gain() * frame->read(Modulation::SAMPLE_GAIN);
			//	sample_.set_start(sample_src_->start() * frame->data[Modulation::START]);
			//	sample_.set_end(sample_src_->end() * frame->data[Modulation::END]);
			//	sample_.set_loop_start(sample_src_->loop_start() * frame->data[Modulation::LOOP_START]);
			//	sample_.set_loop_end(sample_src_->loop_end() * frame->data[Modulation::LOOP_END]);
		}
	}

};

#endif
