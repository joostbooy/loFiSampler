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

	void init(Settings *settings, ModulationEngine *modulationEngine) {
		state_ = IDLE;
		key_pressed_ = false;
		stop_requested_ = false;
		modulationEngine_ = modulationEngine;
		envelopeEngine_[0].init(&settings->envelope(0));
		envelopeEngine_[1].init(&settings->envelope(1));
	}

	uint8_t port() { return port_; }
	uint8_t note() { return note_; }
	uint32_t phase() { return phase_; }
	uint8_t channel() { return channel_; }
	bool key_pressed() { return key_pressed_; }
	State state() { return state_; }
	Sample &sample() { return sample_; }
	EnvelopeEngine &envelopeEngine(int index) { return envelopeEngine_[index]; }

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

		left_ = 0;
		right_ = 0;
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

		envelopeEngine_[0].attack();
		envelopeEngine_[1].attack();
	}

	void note_off() {
		key_pressed_ = false;
		envelopeEngine_[0].release();
		envelopeEngine_[1].release();
	}

	void fill(Dac::Channel *channel, size_t size) {
		sample_.paste(sample_src_);
		instrument_.paste(instrument_src_);

		apply_modulation();

		int16_t *l_ptr = &channel[instrument_.audio_channel()].left[0];
		int16_t *r_ptr = &channel[instrument_.audio_channel()].right[0];

		int shifts = 16 - instrument_.bit_depth();
		float gain = sample_.gain() * instrument_.gain();
		float inc = get_inc(note_, sample_.root_note(), instrument_.bend_range(), instrument_.bend(), sample_.cents());

		while (size--) {
			uint32_t intergral = static_cast<uint32_t>(phase_);
			float fractional = phase_ - intergral;

			int16_t l, r;
			int16_t l_next, r_next;

			sample_.read(intergral, &l, &r);
			sample_.read(intergral + state_, &l_next, &r_next);
			l = Dsp::cross_fade(l, l_next, fractional);
			r = Dsp::cross_fade(r, r_next, fractional);
			l = gain * ((l >> shifts) << shifts);
			r = gain * ((r >> shifts) << shifts);

			Dsp::pan(&l, &r, sample_.pan());

			phase_ += inc;
			update_state(phase_);

			if (++sample_count_ >= instrument_.sample_rate_divider()) {
				sample_count_ = 0;
				left_ = l;
				right_ = r;
			}

			*l_ptr++ += left_;
			*r_ptr++ += right_;
		}
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
	ModulationEngine *modulationEngine_;
	EnvelopeEngine envelopeEngine_[Settings::kNumEnvelopes];
	int16_t left_, right_;
	int sample_count_;

	inline void update_state(uint32_t phase) {
		if (state_ == FORWARD) {
			if (is_looping()) {
				next_loop_forward(phase);
			} else {
				next_forward(phase);
			}
		} else if (state_ == BACKWARD) {
			if (is_looping()) {
				next_loop_backward(phase);
			} else {
				next_backward(phase);
			}
		}
	}

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

	inline float get_inc(int note, int root_note, int bend_range, float bend, int cents) {
		int semitone = 128 - (note - root_note);
		float a = lut_semitone_ratio[SettingsUtils::clip_min(0, semitone - bend_range)];
		float b = lut_semitone_ratio[semitone];
		float c = lut_semitone_ratio[SettingsUtils::clip_max(255, semitone + bend_range)];
		return Dsp::cross_fade(a, b, c, bend) * lut_cent_ratio[cents + 99] * state_;
	}

	inline void apply_modulation() {
		modulationEngine_->set_midi_velocity(velocity_);
		modulationEngine_->set_envelope(0, envelopeEngine_[0].next());
		modulationEngine_->set_envelope(1, envelopeEngine_[1].next());
		ModulationEngine::Frame *frame = modulationEngine_->process(&instrument_src_->modulationMatrix());

		instrument_.set_bend(instrument_src_->bend() * frame->data[ModulationMatrix::BEND]);
		instrument_.set_bit_depth(instrument_src_->bit_depth() * frame->data[ModulationMatrix::BIT_DEPTH]);
		instrument_.set_sample_rate_divider(instrument_src_->sample_rate_divider() * frame->data[ModulationMatrix::SAMPLE_RATE_DIVIDER]);
		sample_.set_start(sample_src_->start() * frame->data[ModulationMatrix::START]);
		sample_.set_end(sample_src_->end() * frame->data[ModulationMatrix::END]);
		sample_.set_loop_start(sample_src_->loop_start() * frame->data[ModulationMatrix::LOOP_START]);
		sample_.set_loop_end(sample_src_->loop_end() * frame->data[ModulationMatrix::LOOP_END]);
		sample_.set_pan(sample_src_->pan() * frame->data[ModulationMatrix::PAN]);

		if (stop_requested_) {
			if (fade_phase_ > 0.0f) {
				fade_phase_ -= ((Dac::kUpdateRate / 1000) * 4);
			} else {
				stop_requested_ = false;
				state_ = IDLE;
			}
		}
		sample_.set_gain(fade_phase_ * sample_src_->gain() * frame->data[ModulationMatrix::GAIN]);
	}
};

#endif
