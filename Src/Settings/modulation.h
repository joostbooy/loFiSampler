#ifndef Modulation_h
#define Modulation_h

#include "uiText.h"
#include "fileWriter.h"
#include "fileReader.h"
#include "midiEngine.h"

class Modulation {

public:

	static const size_t kNumUserCc = 4;
	static const size_t kNumGatesToNote = 4;

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
		for (size_t i = 0; i < kNumGatesToNote; ++i) {
			set_gate_to_midi_port(i, 0);
			set_gate_to_midi_channel(i, i);
			set_gate_to_midi_note(i, i);
			set_gate_to_midi_velocity(i, 100);
		}

		for (size_t i = 0; i < kNumUserCc; ++i) {
			set_midi_cc_number(i, i);
		}
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

	// Gate to midi
	MidiEngine::Event &gate_to_midi(uint8_t gate) {
		return gate_to_midi_[gate];
	}

	// port
	uint8_t gate_to_midi_port(uint8_t gate) {
		return gate_to_midi_[gate].port;
	}

	void set_gate_to_midi_port(uint8_t gate, uint8_t port) {
		gate_to_midi_[gate].port = port;
	}

	const char *gate_to_midi_port_text(uint8_t gate) {
		return MidiEngine::port_text(gate_to_midi_port(gate));
	}


	// channel
	uint8_t gate_to_midi_channel(uint8_t gate) {
		return gate_to_midi_[gate].message;
	}

	void set_gate_to_midi_channel(uint8_t gate, uint8_t chn) {
		gate_to_midi_[gate].message = chn;
	}

	const char *gate_to_midi_channel_text(uint8_t gate) {
		return UiText::str.write("CHN ", gate_to_midi_channel(gate) + 1);
	}

	// note
	uint8_t gate_to_midi_note(uint8_t gate) {
		return gate_to_midi_[gate].data[0];
	}

	void set_gate_to_midi_note(uint8_t gate, uint8_t note) {
		gate_to_midi_[gate].data[0] = note;
	}

	const char *gate_to_midi_note_text(uint8_t gate) {
		return UiText::note_to_text(gate_to_midi_note(gate));
	}

	// velocity
	uint8_t gate_to_midi_velocity(uint8_t gate) {
		return gate_to_midi_[gate].data[1];
	}

	void set_gate_to_midi_velocity(uint8_t gate, uint8_t vel) {
		gate_to_midi_[gate].data[1] = vel;
	}

	const char *gate_to_midi_velocity_text(uint8_t gate) {
		return UiText::str.write("VEL ", gate_to_midi_velocity(gate));
	}


	// Storage
	void save(FileWriter &fileWriter) {
		for (size_t i = 0; i < kNumUserCc; ++i) {
			fileWriter.write(midi_cc_number_[i]);
		}

		for (size_t i = 0; i < kNumGatesToNote; ++i) {
			fileWriter.write(gate_to_midi_[i]);
		}
	}

	void load(FileReader &fileReader) {
		for (size_t i = 0; i < kNumUserCc; ++i) {
			fileReader.read(midi_cc_number_[i]);
		}

		for (size_t i = 0; i < kNumGatesToNote; ++i) {
			fileReader.read(gate_to_midi_[i]);
		}
	}

private:
	uint8_t midi_cc_number_[kNumUserCc];
	MidiEngine::Event gate_to_midi_[kNumGatesToNote];
};

#endif
