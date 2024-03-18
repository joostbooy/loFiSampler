#ifndef Modulation_h
#define Modulation_h

#include "uiText.h"
#include "fileWriter.h"
#include "fileReader.h"
#include "midi.h"

class Modulation {

public:

	enum Destination {
		GAIN,
		PAN,
		BEND,
		START,
		END,
		LOOP_START,
		LOOP_END,

		NUM_DESTINATIONS
	};

	const char *destination_text(int value) {
		switch (value)
		{
		case GAIN:			return "GAIN";
		case PAN:			return "PAN";
		case BEND:			return "BEND";
		case START:			return "START";
		case END:			return "END";
		case LOOP_START:	return "LOOP START";
		case LOOP_END:		return "LOOP END";
		default:
			break;
		}
		return nullptr;
	}

	enum Sources {
		LFO_1,
		LFO_2,
		LFO_3,
		LFO_4,
		CV_1,
		CV_2,
		CV_3,
		CV_4,
		MIDI_MOD,
		MIDI_BEND,
		MIDI_CC_A,
		MIDI_CC_B,
		MIDI_CC_C,
		MIDI_CC_D,
		MOD_ENVELOPE,

		NUM_SOURCES
	};

	const char *source_text(int value) {
		switch (value)
		{
		case LFO_1:			return "LFO 1";
		case LFO_2:			return "LFO 2";
		case LFO_3:			return "LFO 3";
		case LFO_4:			return "LFO 4";
		case CV_1:			return "CV 1";
		case CV_2:			return "CV 3";
		case CV_3:			return "CV 3";
		case CV_4:			return "CV 4";
		case MIDI_MOD:		return "MIDI_MOD";
		case MIDI_BEND:		return "MIDI_BEND";
		case MIDI_CC_A:		return midi_cc_number_text(0);
		case MIDI_CC_B:		return midi_cc_number_text(1);
		case MIDI_CC_C:		return midi_cc_number_text(2);
		case MIDI_CC_D:		return midi_cc_number_text(3);
		case MOD_ENVELOPE:	return "MOD ENVELOPE";
		default:
			break;
		}
		return nullptr;
	}

	void init() {
		for (size_t i = 0; i < kNumUserCc; ++i) {
			set_midi_cc_number(i, i);
		}

		for (size_t i = 0; i < NUM_SOURCES; ++i) {
			matrix_[i] = 0;
		}
	}

	// Midi CC
	int midi_cc_number(int index) {
		return midi_cc_number_[index];
	}

	void set_midi_cc_number(int index, int cc_number) {
		midi_cc_number_[index] = cc_number;
	}

	const char* midi_cc_number_text(int index) {
		return UiText::str.write("MIDI CC ", midi_cc_number(index));
	}

	static constexpr const size_t num_user_cc() {
		return kNumUserCc;
	}

	// Matrix
	bool read_matrix(int src, int dest) {
		return matrix_[src] & (1 << dest);
	}

	void write_matrix(int src, int dest, bool state) {
		if (state) {
			matrix_[src] |= (1 << dest);
		} else {
			matrix_[src] &= ~(1 << dest);
		}
	}

	// Storage
	void save(FileWriter &fileWriter) {
		for (size_t i = 0; i < kNumUserCc; ++i) {
			fileWriter.write(midi_cc_number_[i]);
		}

		for (size_t i = 0; i < NUM_SOURCES; ++i) {
			fileWriter.write(matrix_[i]);
		}
	}

	void load(FileReader &fileReader) {
		for (size_t i = 0; i < kNumUserCc; ++i) {
			fileReader.read(midi_cc_number_[i]);
		}

		for (size_t i = 0; i < NUM_SOURCES; ++i) {
			fileReader.read(matrix_[i]);
		}
	}

private:
	static const size_t kNumUserCc = 4;
	uint8_t midi_cc_number_[kNumUserCc];
	uint32_t matrix_[NUM_SOURCES];
};

#endif
