#ifndef Midi_h
#define Midi_h

#include "fileWriter.h"
#include "fileReader.h"
#include "lookupTables.h"
#include "uiText.h"

class Midi {

public:

	enum Port {
		UART,
		USB,

		NUM_PORTS
	};

	static const char *port_text(int port) {
		switch (port)
		{
		case UART:	return "UART";
		case USB:	return "USB";
		default:
			break;
		}
		return nullptr;
	}


	enum ClockSource {
		EXTERNAL = NUM_PORTS,
		INTERNAL,

		NUM_CLOCK_SOURCES
	};

	static const char* clock_source_text(int value) {
		if (value < INTERNAL) {
			return Midi::port_text(value);
		} else {
			return "INTERNAL";
		}
	}

	void init() {
		set_bpm(120);
		set_clock_source(INTERNAL);

		for (int i = 0; i < NUM_PORTS; ++i){
			set_send_clock(i, true);
		}
	}

	// bpm
	uint16_t bpm() {
		return bpm_;
	}

	void set_bpm(int value) {
		bpm_ = stmlib::clip(1, MAX_BPM - 1, value);
	}

	const char *bpm_text() {
		return UiText::str.write(bpm(), " BPM");
	}

	// clock source
	uint8_t clock_source() {
		return clock_source_;
	}

	void set_clock_source(int value) {
		clock_source_ = stmlib::clip(0, NUM_CLOCK_SOURCES - 1, value);
	}

	const char* clock_source_text() {
		return clock_source_text(clock_source());
	}

	// clock source
	bool send_clock(int port) {
		return send_clock_[port];
	}

	void set_send_clock(int port, bool value) {
		send_clock_[port] = value;
	}

	const char* send_clock_text(int port) {
		return UiText::bool_to_on_off(send_clock(port));
	}

	// Storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(bpm_);
		fileWriter.write(clock_source_);

		for (int i = 0; i < NUM_PORTS; ++i) {
			fileWriter.write(send_clock_[i]);
		}
	}

	void load(FileReader &fileReader) {
		fileReader.read(bpm_);
		fileReader.read(clock_source_);

		for (int i = 0; i < NUM_PORTS; ++i) {
			fileReader.read(send_clock_[i]);
		}
	}

private:
	uint16_t bpm_;
	uint8_t clock_source_;
	bool send_clock_[NUM_PORTS];
};

#endif
