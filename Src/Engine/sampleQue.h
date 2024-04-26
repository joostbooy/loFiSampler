#ifndef SampleQue_h
#define SampleQue_h

#include "instrument.h"
#include "sample.h"
#include "midiEngine.h"

class SampleQue {

public:

	struct Event {
		Instrument *instrument_;
		Sample *sample_;
		MidiEngine::Event midi_event_;
	};

	void init() {
		event_que_.init();
	}

	bool readable() {
		return event_que_.readable();
	}

	Event read() {
		return event_que_.read();
	}

	size_t note_on(MidiEngine::Event &e) {
		size_t last_size = event_que_.size();

		for (size_t i = 0; i < Settings::kMaxInstruments; ++i) {
			write(&settings.instrument(i), e);
		}

		return event_que_.size() - last_size;
	}

private:
	Que<Event, 8>event_que_;

	void write(Instrument *instrument, MidiEngine::Event &e) {
		if (instrument->midi_accepted(e)) {
			for (size_t i = 0; i < instrument->num_samples(); ++i) {
				if (instrument->sample(i)->within_key_range(e.data[0]) && event_que_.writeable()) {
					event_que_.write( { .instrument_ = instrument, .sample_ = instrument->sample(i), .midi_event_ = e } );
				}
			}
		}
	}
};

#endif