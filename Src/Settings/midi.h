#ifndef Midi_h
#define Midi_h

#include "fileWriter.h"
#include "fileReader.h"
#include "uiText.h"
#include "lookupTables.h"

class Midi {

public:

	enum ClockSource {
		UART,
		USB,
		INTERNAL,
		ANALOG,

		NUM_CLOCK_SOURCES
	};

	static const char* clock_source_text(int value) {
		switch (value)
		{
		case UART:		return "UART";
		case USB:		return "USB";
		case INTERNAL:	return "INTERNAL";
		case ANALOG:	return "ANALOG";
		default:		break;
		}
		return nullptr;
	}

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

	void init() {
		set_bpm(120);
		set_clock_source(INTERNAL);
		set_send_clock(true);
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
	bool send_clock() {
		return send_clock_;
	}

	void set_send_clock(bool value) {
		send_clock_ = value;
	}

	const char* send_clock_text() {
		return UiText::bool_to_on_off(send_clock());
	}

	// Tempo phase inc
	static const char *tempo_text(int steps, int type) {
		return UiText::str.write("1/", steps, tempo_type_text(type));
	}

	static float read_tempo_phase_inc(int bpm, int steps, int type) {
		return steps * lut_tempo_sync_phase_inc[bpm] * tempo_type_multiplier(type);
	}


	// Storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(bpm_);
		fileWriter.write(clock_source_);
		fileWriter.write(send_clock_);
	}

	void load(FileReader &fileReader) {
		fileReader.read(bpm_);
		fileReader.read(clock_source_);
		fileReader.read(send_clock_);
	}

private:
	uint16_t bpm_;
	uint8_t clock_source_;
	bool send_clock_;
	//constexpr const float tempo_type_multiplier_[NUM_TYPES] = { 1.f / 1.5f, 1.f, 1.5f };

	static const float tempo_type_multiplier(uint8_t type) {
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
