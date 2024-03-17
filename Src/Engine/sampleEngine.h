#ifndef SampleEngine_h
#define SampleEngine_h

#include "sample.h"
#include "modulationEngine.h"

class SampleEngine {

public:

	void init() {
		is_playing_ = false;
	}

	void note_on(MidiEngine::Event &e, Sample *sample, Instrument *instrument) {
		sample_ = sample;
		instrument_ = instrument;
		key_pressed_ = true;
		is_playing_ = true;

		note_ = e.data[0];
		velocity_ = (1.f / 127.f) * e.data[1];

		if ((sample_->play_mode() == Sample::FORWARD) || (sample_->play_mode() == Sample::PING_PONG)) {
			phase_ = sample_->start();
			direction_ = 1;
		} else {
			phase_ = sample_->end();
			direction_ = -1;
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
		if (is_playing_ == false) {
			return 0;
		}

		uint32_t intergral = static_cast<uint32_t>(phase_);
		float fractional = phase_ - intergral;

		uint8_t shifts = instrument_->bit_shifts();
		int16_t a = sample_->data(phase_intergral);
		int16_t b = sample_->data(phase_intergral + direction_);
		int16_t value = (Dsp::cross_fade(a, b, fractional) >> shifts) << shifts;

		phase_ += get_inc(note_, sample_->root_note(), instrument_->bend_range(), frame->bend);

		switch (sample_->play_mode())
		{
		case Sample::FORWARD:
			if (!next_phase_forward()) {
				is_playing_ = false;
			}
			break;
		case Sample::BACKWARD:
			if (!next_phase_backward()) {
				is_playing_ = false;
			}
			break;
		case Sample::PING_PONG:
			if (direction_ == 1) {
				if (!next_phase_forward(intergral)) {
					direction_ = -1;
				}
			} else {
				if (!next_phase_backward(intergral)) {
					is_playing_ = false;
				}
			}
			break;
		case Sample::PING_PONG_REVERSE:
			if (direction_ == -1) {
				if (!next_phase_backward(intergral)) {
					direction_ = 1;
				}
			} else {
				if (!next_phase_forward(intergral)) {
					is_playing_ = false;
				}
			}
			break;
		default:
			break;
		}

		//float pan_value = sample_->pan() * frame->pan();
		//Dsp::pan(pan_value);

		return value * velocity_ * frame->gain;
	}

private:
	float phase_;
	float velocity_;
	uint8_t note_;
	bool key_pressed_;
	bool is_playing_;
	int direction_;
	Sample_ *sample_;
	Instrument *instrument_;

	inline bool next_phase_forward(uint32_t intergral) {
		if (sample_->loop() && key_pressed_ && (intergral >= sample_->loop_end())) {
			phase_ = sample_->loop_start();
		} else if (intergral >= sample_->end()) {
			return false;
		}
		return true;
	}

	inline bool next_phase_backward(uint32_t intergral) {
		if (sample_->loop() && key_pressed_ && (intergral <= sample_->loop_start())) {
			phase_ = sample_->loop_end();
		} else if (intergral <= sample_->start()) {
			return false;
		}
		return true;
	}

	inline float get_inc(int note, int root_note, int bend_range, float bend = 0.5f) {
		int index = 128 - (note - root_note);
		float a = lut_frequency_ratio[stmlib::clip_max(255, index + bend_range)];
		float b = lut_frequency_ratio[index];
		float c = lut_frequency_ratio[stmlib::clip_min(0, index - bend_range)];
		return Dsp::cross_fade(a, b, c, bend) * direction_;
	}
};

#endif
