#ifndef Voice_h
#define Voice_h

class Voice {

public:

	void init() {
		stop_requested_= false;

		sampleEngine_.init();
		envelopeEngine_[0].init(&settings.envelope(0));
		envelopeEngine_[1].init(&settings.envelope(1));
	}

	uint8_t port() {
		return event_.midi_event_.port;
	}

	uint8_t velocity() {
		return event_.midi_event_.data[1];
	}

	uint8_t note() {
		return event_.midi_event_.data[0];
	}

	uint8_t channel() {
		return event_.midi_event_.message & 0x0F;
	}

	void note_on(SampleQue::Event &event) {
		event_ = event;
		envelopeEngine_[0].attack();
		envelopeEngine_[1].attack();
		sampleEngine_.note_on(event);
	}

	void note_off() {
		envelopeEngine_[0].release();
		envelopeEngine_[1].release();
		sampleEngine_.note_off();
	}

	void request_stop() {
		stop_requested_ = true;
	}

	bool is_available() {
		return (sampleEngine_.state() == SampleEngine::IDLE); // gain_ == 0.0f
	}

private:
	bool stop_requested_;
	SampleQue::Event event_;
	SampleEngine sampleEngine_;
	EnvelopeEngine envelopeEngine_[2];
};

#endif
