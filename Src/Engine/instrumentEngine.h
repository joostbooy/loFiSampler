#ifndef InstrumentEngine_h
#define InstrumentEngine_h

class InstrumentEngine {

public:

	void init(Instument *instument) {
		instument_ = instument;
	}

	void note_on(MidiEngine::Event &e) {
		if (instument_->midi_port_accepted(e.port) && instument_->midi_channel_accepted(e.message & 0x0F)) {
			for (int i = 0; i < instument_->num_samples(); ++i) {
				if (instument_->sample(i).within_key_range(e.data[0])) {
					sample_que.write(instument_->sample(i));
				}
			}
		}
	}

	void note_off(MidiEngine::Event &e) {

	}

	//	bool process(Sample *sample) {
	//		if (sample_que.readable()) {
	//			*sample = sample_que.read();
	//			return true;
	//		}
	//		return false;
	//	}


private:
	Instument *instument_;
	Que<Sample*, 8>sample_que;
};

#endif
