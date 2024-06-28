#ifndef ClipBoard_h
#define ClipBoard_h

#include "lfo.h"
#include "envelope.h"

class ClipBoard {

public:

	void init() {
		lfo_pastable_ = false;
		envelope_pastable_ = false;
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

private:
	Lfo lfo_;
	Envelope envelope_;
	bool envelope_pastable_;
	bool lfo_pastable_;
};

extern ClipBoard clipboard;


#endif
