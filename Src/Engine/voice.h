#ifndef Voice_h
#define Voice_h

#include "sample.h"
#include "sampleQue.h"
#include "envelopeEngine.h"
#include "modulationEngine.h"

class Voice {

public:

	enum State {
		BACKWARD 	= -1,
		IDLE		= 0,
		FORWARD		= 1
	};

	void init() {
		state_ = IDLE;
		key_pressed_ = false;
		stop_requested_ = false;
		envelope_[0].init(&settings.envelope(0));
		envelope_[1].init(&settings.envelope(1));
	}

	uint8_t port() { return port_; }
	uint8_t note() { return note_; }
	uint32_t phase() { return phase_; }
	uint8_t channel() { return channel_; }
	bool key_pressed() { return key_pressed_; }
	State state() { return state_; }
	Sample &sample() { return sample_; }

	void request_stop() {
		stop_requested_ = true;
	}

	bool is_available() {
		return state_ == IDLE;
	}

	void note_on(SampleQue::Event &e) {
		sample_src_ = e.sample_;
		instrument_src_ = e.instrument_;
		port_ = e.midi_event_.port;
		note_ = e.midi_event_.data[0];
		channel_ = e.midi_event_.message & 0x0F;
		velocity_ = e.midi_event_.data[1] * (1.f / 127.f);

		envelope_[0].attack();
		envelope_[1].attack();

		key_pressed_ = true;
		stop_requested_ = false;
		fade_phase_  = 1.f;

		if (sample_src_->play_mode() == Sample::FORWARD) {
			phase_ = sample_src_->start();
			state_ = FORWARD;
		} else {
			phase_ = sample_src_->end();
			state_ = BACKWARD;
		}
	}

	void note_off() {
		key_pressed_ = false;
		envelope_[0].release();
		envelope_[1].release();
	}

	void fill(Dac::Buffer *buffer, const size_t size) {
		sample_.paste(sample_src_);
		instrument_.paste(instrument_src_);

		apply_modulation();

		int16_t *ptr = &buffer[0].channel[instrument_.audio_channel() * 2];

		for (size_t i = 0; i < size; ++i) {
			int16_t left = next();
			int16_t right = next();
			Dsp::pan(&left, &right, instrument_.pan());

			*ptr += left;
			*(ptr + 1) += right;

			ptr += Dac::kNumChannels;
		}
	}

	int16_t next() {
		uint32_t intergral = static_cast<uint32_t>(phase_);
		float fractional = phase_ - intergral;

		int16_t *a = sample_.data(intergral);
		int16_t *b = a + (sample_.num_channels() * state_));

		uint8_t shifts = instrument_.bit_shifts();
		int16_t value = (Dsp::cross_fade(*a, *b, fractional) >> shifts) << shifts;

		phase_ += get_inc(note_, sample_.root_note(), instrument_.bend_range(), sample_.cents(), 0.5f);

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

		return value * sample_.gain() * instrument_.gain();
	}

private:
	float phase_;
	bool key_pressed_;
	bool stop_requested_;
	uint8_t note_;
	uint8_t port_;
	uint8_t channel_;
	State state_;
	float velocity_;
	float fade_phase_;
	Sample sample_;
	Sample *sample_src_;
	Instrument instrument_;
	Instrument *instrument_src_;
	ModulationEngine *modualationEngine_;
	EnvelopeEngine envelope_[Settings::kNumEnvelopes];

	inline bool is_looping() {
		return sample_.loop() && key_pressed_;
	}

	inline void next_forward(uint32_t phase) {
		if (phase >= sample_.end()) {
			if (sample_.u_turn() && (sample_.play_mode() == Sample::FORWARD)) {
				state_ = BACKWARD;
			} else {
				state_ = IDLE;
			}
		}
	}

	inline void next_backward(uint32_t phase) {
		if (phase <= sample_.start()) {
			if (sample_.u_turn() && (sample_.play_mode() == Sample::BACKWARD)) {
				state_ = FORWARD;
			} else {
				state_ = IDLE;
			}
		}
	}

	inline void next_loop_forward(uint32_t phase) {
		if (phase >= sample_.loop_end()) {
			if (sample_.u_turn()) {
				state_ = BACKWARD;
			} else {
				phase_ = sample_.loop_start();
			}
		}
	}

	inline void next_loop_backward(uint32_t phase) {
		if (phase <= sample_.loop_start()) {
			if (sample_.u_turn()) {
				state_ = FORWARD;
			} else {
				phase_ = sample_.loop_end();
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

	inline void apply_modulation() {
		modualationEngine_->set_midi_velocity(velocity_);
		modualationEngine_->set_envelope(0, envelope_[0].next());
		modualationEngine_->set_envelope(1, envelope_[1].next());
		ModulationEngine::Frame *frame = modualationEngine_->process(&instrument_src_->matrix());

		sample_.set_start(sample_src_->start() * frame->data[Modulation::START]);
		sample_.set_end(sample_src_->end() * frame->data[Modulation::END]);
		sample_.set_loop_start(sample_src_->loop_start() * frame->data[Modulation::LOOP_START]);
		sample_.set_loop_end(sample_src_->loop_end() * frame->data[Modulation::LOOP_END]);
		instrument_.set_pan(instrument_src_->pan() * frame->data[Modulation::PAN]);
		instrument_.set_gain(instrument_src_->gain() * frame->data[Modulation::INSTRUMENT_GAIN]);
		instrument_.set_bit_depth(instrument_src_->bit_depth() * frame->data[Modulation::BIT_DEPTH]);

		if (stop_requested_) {
			if (fade_phase_ > 0.0f) {
				fade_phase_ -= ((Dac::kUpdateRate / 1000) * 4);
			} else {
				stop_requested_ = false;
				state_ = IDLE;
			}
		}
		sample_.set_gain(fade_phase_ * sample_src_->gain() * frame->data[Modulation::SAMPLE_GAIN]);
	}
};

#endif
