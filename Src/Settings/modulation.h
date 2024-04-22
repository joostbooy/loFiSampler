#ifndef Modulation_h
#define Modulation_h

#include "uiText.h"
#include "fileWriter.h"
#include "fileReader.h"

class Modulation {

public:

	enum Destination {
		PAN,
		BEND,
		START,
		END,
		LOOP_START,
		LOOP_END,
		SAMPLE_GAIN,
		INSTRUMENT_GAIN,
		BIT_DEPTH,

		NUM_DESTINATIONS
	};

	static_assert(NUM_DESTINATIONS < 32, "Too many destinations !");

	const char *destination_text(int value) {
		switch (value)
		{
		case PAN:				return "PAN";
		case BEND:				return "BEND";
		case START:				return "START";
		case END:				return "END";
		case LOOP_START:		return "LOOP START";
		case LOOP_END:			return "LOOP END";
		case SAMPLE_GAIN:		return "SAMPLE GAIN";
		case INSTRUMENT_GAIN:	return "INSTRUMENT GAIN";
		case BIT_DEPTH:			return "BIT DEPTH";
		default:
			break;
		}
		return nullptr;
	}

	enum Source {
		LFO_1,
		LFO_2,
		LFO_3,
		LFO_4,
		CV_1,
		CV_2,
		CV_3,
		CV_4,
		MIDI_BEND,
		MIDI_VELOCITY,
		MIDI_CC_A,
		MIDI_CC_B,
		MIDI_CC_C,
		MIDI_CC_D,
		ENVELOPE_1,
		ENVELOPE_2,

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
		case CV_2:			return "CV 2";
		case CV_3:			return "CV 3";
		case CV_4:			return "CV 4";
		case MIDI_BEND:		return "MIDI BEND";
		case MIDI_VELOCITY:	return "MIDI VELOCITY";
		case MIDI_CC_A:		return midi_cc_number_text(0);
		case MIDI_CC_B:		return midi_cc_number_text(1);
		case MIDI_CC_C:		return midi_cc_number_text(2);
		case MIDI_CC_D:		return midi_cc_number_text(3);
		case ENVELOPE_1:	return "ENVELOPE 1";
		case ENVELOPE_2:	return "ENVELOPE 2";
		default:
			break;
		}
		return nullptr;
	}

	void init() {
		for (size_t i = 0; i < kNumUserCc; ++i) {
			set_midi_cc_number(i, i);
		}

		clear_matrix();
		write_matrix(MIDI_VELOCITY, SAMPLE_GAIN, true);
		write_matrix(ENVELOPE_1, SAMPLE_GAIN, true);
	}

	// Midi CC
	int midi_cc_number(int index) {
		return midi_cc_number_[index];
	}

	void set_midi_cc_number(int index, int cc_number) {
		midi_cc_number_[index] = stmlib::clip(0, 127, cc_number);
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
		uint32_t data = matrix_[src];

		if (state) {
			matrix_[src] = data | (1 << dest);
		} else {
			matrix_[src] = data & ~(1 << dest);
		}
	}

	void clear_matrix() {
		for (size_t i = 0; i < NUM_SOURCES; ++i) {
			matrix_[i] = 0;
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
