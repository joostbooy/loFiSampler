#ifndef MidiSync_h
#define MidiSync_h

#include "settingsText.h"
#include "midiClockEngine.h"

class MidiSync {

public:

	enum Tempo {
		_1_1_DOTTED,
		_1_1_WHOLE,
		_1_1_TRIPLET,
		_1_2_DOTTED,
		_1_2_WHOLE,
		_1_2_TRIPLET,
		_1_4_DOTTED,
		_1_4_WHOLE,
		_1_4_TRIPLET,
		_1_8_DOTTED,
		_1_8_WHOLE,
		_1_8_TRIPLET,
		_1_16_DOTTED,
		_1_16_WHOLE,
		_1_16_TRIPLET,
		_1_32_DOTTED,
		_1_32_WHOLE,
		_1_32_TRIPLET,
		_1_64_DOTTED,
		_1_64_WHOLE,
		_1_64_TRIPLET,

		NUM_TEMPOS
	};

	static const char* tempo_text(int value) {
		switch (value)
		{
		case _1_1_DOTTED:	return "1/1 D";
		case _1_1_WHOLE:	return "1/1";
		case _1_1_TRIPLET:	return "1/1 T";
		case _1_2_DOTTED:	return "1/2 D";
		case _1_2_WHOLE:	return "1/2";
		case _1_2_TRIPLET:	return "1/2 T";
		case _1_4_DOTTED:	return "1/4 D";
		case _1_4_WHOLE:	return "1/4";
		case _1_4_TRIPLET:	return "1/4 T";
		case _1_8_DOTTED:	return "1/8 D";
		case _1_8_WHOLE:	return "1/8";
		case _1_8_TRIPLET:	return "1/8 T";
		case _1_16_DOTTED:	return "1/16 D";
		case _1_16_WHOLE:	return "1/16";
		case _1_16_TRIPLET:	return "1/16 T";
		case _1_32_DOTTED:	return "1/32 D";
		case _1_32_WHOLE:	return "1/32";
		case _1_32_TRIPLET:	return "1/32 T";
		case _1_64_DOTTED:	return "1/64 D";
		case _1_64_WHOLE:	return "1/64";
		case _1_64_TRIPLET:	return "1/64 T";
		default:		break;
		}
		return nullptr;
	}

	static size_t read_beat_length(int value) {
		return lut_beat_length[MidiClockEngine::bpm()] * beat_multiplier(value);
	}

	static float read_inc(int value) {
		return lut_sync_phase_inc[MidiClockEngine::bpm()] * inc_multiplier(value);
	}

private:

	static float beat_multiplier(int type) {
		switch (type)
		{
		case _1_1_DOTTED:	return 6.f;
		case _1_1_WHOLE:	return 4.f;
		case _1_1_TRIPLET:	return 4.f / 1.5f;
		case _1_2_DOTTED:	return 3.f;
		case _1_2_WHOLE:	return 2.f;
		case _1_2_TRIPLET:	return 2.f / 1.5f;
		case _1_4_DOTTED:	return 1.5f;
		case _1_4_WHOLE:	return 1.f;
		case _1_4_TRIPLET:	return 1.f / 1.5f;
		case _1_8_DOTTED:	return 0.75f;
		case _1_8_WHOLE:	return 0.5f;
		case _1_8_TRIPLET:	return 0.5f / 1.5f;
		case _1_16_DOTTED:	return 0.375f;
		case _1_16_WHOLE:	return 0.25f;
		case _1_16_TRIPLET:	return 0.25f / 1.5f;
		case _1_32_DOTTED:	return 0.1875f;
		case _1_32_WHOLE:	return 0.125f;
		case _1_32_TRIPLET:	return 0.125f / 1.5f;
		case _1_64_DOTTED:	return 0.09375f;
		case _1_64_WHOLE:	return 0.0625f;
		case _1_64_TRIPLET:	return 0.0625f / 1.5f;
		default:
			break;
		}
		return 0.f;
	}

	static float inc_multiplier(int type) {
		switch (type)
		{
		case _1_1_DOTTED:	return 0.25f / 1.5f;
		case _1_1_WHOLE:	return 0.25f;
		case _1_1_TRIPLET:	return 0.375f;
		case _1_2_DOTTED:	return 0.5f / 1.5f;
		case _1_2_WHOLE:	return 0.5f;
		case _1_2_TRIPLET:	return 0.75f;
		case _1_4_DOTTED:	return 1.f / 1.5f;
		case _1_4_WHOLE:	return 1.f;
		case _1_4_TRIPLET:	return 1.5f;
		case _1_8_DOTTED:	return 2.f / 1.5f;
		case _1_8_WHOLE:	return 2.f;
		case _1_8_TRIPLET:	return 3.f;
		case _1_16_DOTTED:	return 4.f / 1.5f;
		case _1_16_WHOLE:	return 4.f;
		case _1_16_TRIPLET:	return 6.f;
		case _1_32_DOTTED:	return 8.f / 1.5f;
		case _1_32_WHOLE:	return 8.f;
		case _1_32_TRIPLET:	return 12.f;
		case _1_64_DOTTED:	return 16.f / 1.5f;
		case _1_64_WHOLE:	return 16.f;
		case _1_64_TRIPLET:	return 24.f;
		default:
			break;
		}
		return 0.f;
	}
};

#endif
