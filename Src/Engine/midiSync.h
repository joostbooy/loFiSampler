#ifndef MidiSync_h
#define MidiSync_h

#include "uiText.h"
#include "midiClockEngine.h"

class MidiSync {

public:

	enum TempoType {
		DOTTED,
		WHOLE,
		TRIPLET,

		NUM_TYPES
	};

	static const char* tempo_type_text(int value) {
		switch (value)
		{
		case DOTTED:	return "DOTTED";
		case WHOLE:		return "WHOLE";
		case TRIPLET:	return "TRIPLET";
		default:		break;
		}
		return nullptr;
	}

	static const size_t max_value() {
		return 16 * NUM_TYPES;
	}

	static const char *tempo_text(int value) {
		int type = value % 3;
		int divisor = (value / 3) + 1;
		return UiText::str.write("1/", divisor, tempo_type_text(type));
	}

	static float read_inc(int value) {
		int type = value % 3;
		int divisor = (value / 3) + 1;
		return divisor * lut_tempo_sync_phase_inc[MidiClockEngine::bpm()] * tempo_type_multiplier(type);
	}

private:

	static float tempo_type_multiplier(int type) {
		switch (type)
		{
		case DOTTED:	return 1.f / 1.5f;
		case WHOLE:		return 1.f;
		case TRIPLET:	return 1.5f;
		default:
			break;
		}
		return 0.f;
	}

};

#endif
