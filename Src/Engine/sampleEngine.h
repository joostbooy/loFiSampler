#ifndef SampleEngine_h
#define SampleEngine_h

#include "sample.h"

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

		if (sample_->play_mode() == Sample::FORWARD || Sample::PING_PONG) {
			phase_ = sample_->start();
			direction_ = 1;
		} else {
			phase_ = sample_->end();
			direction_ = 0;
		}

		velocity_ = (1.f / 127.f) * e.data[1];
		inc_ = lut_frequency_ratio[128 - (e.data[0] - sample_->root_note())];
	}

	void note_off() {
		key_pressed_ = false;
	}

	int16_t next(float bend) {
		if (is_playing_ == false) {
			return 0;
		}

		uint32_t intergral = static_cast<uint32_t>(phase_);
		float fractional = phase_ - intergral;

		uint8_t shifts = instrument_->bit_shifts();
		int16_t a = (sample_->data(phase_intergral) >> shifts) << shifts);
		int16_t b = (sample_->data(phase_intergral + 1) >> shifts) << shifts);
		int16_t value = a + (b - a) * fractional;

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
					direction_ = 0;
				}
			} else {
				if (!next_phase_backward(intergral)) {
					is_playing_ = false;
				}
			}
			break;
		case Sample::PING_PONG_REVERSE:
			if (direction_ == 0) {
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

		return value * velocity_;
	}

private:
	float inc_;
	float phase_;
	float velocity_;
	bool key_pressed_;
	bool is_playing_;
	bool direction_;
	Sample_ *sample_;
	Instrument *instrument_;

	inline bool next_phase_forward(uint32_t intergral) {
		phase_ += inc_;
		if (sample_->loop() && key_pressed_ && (intergral >= sample_->loop_end())) {
			phase_ = sample_->loop_start();
		} else if (intergral >= sample_->end()) {
			return false;
		}
		return true;
	}

	inline bool next_phase_backward(uint32_t intergral) {
		phase_ -= inc_;
		if (sample_->loop() && key_pressed_ && (intergral <= sample_->loop_start())) {
			phase_ = sample_->loop_end();
		} else if (intergral <= sample_->start()) {
			return false;
		}
		return true;
	}

	inline bool next_phase_ping_pong(uint32_t intergral) {
		if (direction_ == 1) {
			phase_ += inc_;
			if (intergral >= sample_->end() || (sample_->loop() && key_pressed_ && (intergral >= sample_->loop_end()))) {
				direction_ = 0;
				phase_ -= inc_;
			}
		} else {
			phase_ -= inc_;
			if (sample_->loop() && key_pressed_ && (intergral <= sample_->loop_start())) {
				direction_ = 1;
				phase_ += inc_;
			} else if (intergral <= sample_->start()) {
				is_playing_ = false;
			}
		}
	}

	inline float inc(int note, int bend_range, float bend = 0.5f) {
		int index = 128 - (note - root_note);
		float a = lut_frequency_ratio[stmlib::clip_max(255, index + bend_range)];
		float b = lut_frequency_ratio[index];
		float c = lut_frequency_ratio[stmlib::clip_min(0, index - bend_range)];
		return Dsp::cross_fade(a, b, c, bend);
	}
};

#endif
