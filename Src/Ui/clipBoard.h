#ifndef ClipBoard_h
#define ClipBoard_h

#include "lfo.h"
#include "envelope.h"
#include "instrument.h"

class ClipBoard {

public:

	void init() {
		lfo_pastable_ = false;
		envelope_pastable_ = false;
		instrument_pasteable_ = false;
	}

	// Envelope
	void copy(Envelope *envelope) {
		envelope_.paste(envelope);
		envelope_pastable_ = true;
	}

	bool paste(Envelope *envelope) {
		if (envelope_pastable()) {
			envelope->paste(&envelope_);
			return true;
		}
		return false;
	}

	bool envelope_pastable() {
		return envelope_pastable_;
	}

	// Lfo
	void copy(Lfo *lfo) {
		lfo_.paste(lfo);
		lfo_pastable_ = true;
	}

	bool paste(Lfo *lfo) {
		if (lfo_pastable()) {
			lfo->paste(&lfo_);
			return true;
		}
		return false;
	}

	bool lfo_pastable() {
		return lfo_pastable_;
	}

	// Instrument
	void copy(Instrument *instument) {
		instrument_.paste(instument);
		instrument_pasteable_ = true;
	}

	bool paste(Instrument *instument) {
		if (instrument_pasteable()) {
			instument->paste(&instrument_);
			return true;
		}
		return false;
	}

	bool instrument_pasteable() {
		return instrument_pasteable_;
	}

private:
	Lfo lfo_;
	Envelope envelope_;
	Instrument instrument_;
	bool envelope_pastable_;
	bool lfo_pastable_;
	bool instrument_pasteable_;
};

extern ClipBoard clipboard;


#endif
