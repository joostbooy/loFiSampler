#ifndef MidiSync_h
#define MidiSync_h

#include "settingsText.h"
#include "midiClockEngine.h"

class MidiSync {

public:

	enum TempoType {
		DOTTED,
		WHOLE,
		TRIPLET,

		NUM_TYPES
	};

	enum Tempo {
		_1_1_WHOLE,
		_1_1_TRIPLET,
		_1_2_WHOLE,
		_1_2_TRIPLET,
		_1_4_WHOLE,
		_1_4_TRIPLET,
		_1_8_WHOLE,
		_1_8_TRIPLET,
		_1_16_WHOLE,
		_1_16_TRIPLET,
		_1_32_WHOLE,
		_1_32_TRIPLET,
		_1_64_WHOLE,
		_1_64_TRIPLET,

		NUM_TEMPOS
	};

	static const char* tempo_text(int value) {
		switch (value)
		{
		case _1_1_WHOLE:	return "1/1";
		case _1_1_TRIPLET:	return "1/1 TRIPLET";
		case _1_2_WHOLE:	return "1/2";
		case _1_2_TRIPLET:	return "1/2 TRIPLET";
		case _1_4_WHOLE:	return "1/4";
		case _1_4_TRIPLET:	return "1/4 TRIPLET";
		case _1_8_WHOLE:	return "1/8";
		case _1_8_TRIPLET:	return "1/8 TRIPLET";
		case _1_16_WHOLE:	return "1/16";
		case _1_16_TRIPLET:	return "1/16 TRIPLET";
		case _1_32_WHOLE:	return "1/32";
		case _1_32_TRIPLET:	return "1/32 TRIPLET";
		case _1_64_WHOLE:	return "1/64";
		case _1_64_TRIPLET:	return "1/64 TRIPLET";
		default:		break;
		}
		return nullptr;
	}

	static float read_inc(int value) {
		return lut_sync_control_rate_inc[MidiClockEngine::bpm()] * beat_multiplier(value);
	}

	static size_t read_num_samples(int value) {
		return lut_beat_length_samples[MidiClockEngine::bpm()] * beat_multiplier(value);
	}

private:

	static float beat_multiplier(int type) {
		switch (type)
		{
		case _1_1_WHOLE:	return 4.f;
		case _1_1_TRIPLET:	return 6.f;
		case _1_2_WHOLE:	return 2.f;
		case _1_2_TRIPLET:	return 3.f;
		case _1_4_WHOLE:	return 1.f;
		case _1_4_TRIPLET:	return 1.5f;
		case _1_8_WHOLE:	return 0.5f;
		case _1_8_TRIPLET:	return 0.75f;
		case _1_16_WHOLE:	return 0.25f;
		case _1_16_TRIPLET:	return 0.375f;
		case _1_32_WHOLE:	return 0.125f;
		case _1_32_TRIPLET:	return 0.1875f;
		case _1_64_WHOLE:	return 0.0625f;
		case _1_64_TRIPLET:	return 0.09375f;
		default:
			break;
		}
		return 0.f;
	}

};

#endif
