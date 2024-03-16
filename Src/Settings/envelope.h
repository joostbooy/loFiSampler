#ifndef Envelope_h
#define Envelope_h

#include "uiText.h"
#include "fileWriter.h"
#include "fileReader.h"

class Envelope {

public:

	enum Mode {
		GATE,
		TRIGGER,

		NUM_MODES
	};

	static const char* mode_text(uint8_t value) {
		switch (value)
		{
		case GATE:		return "GATE";
		case TRIGGER:	return "TRIGGER";
		default:
			break;
		}
		return nullptr;
	}

	void init() {
		set_clock_sync(false);
		set_mode(GATE);
		set_attack_time(0);
		set_attack_shape(0);
		set_decay_time(0);
		set_decay_shape(0);
		set_hold_time(0);
		set_sustain_level(0);
		set_release_time(0);
		set_release_shape(0);
	}

	// Clock sync
	bool clock_sync() {
		return clock_sync_;
	}

	void set_clock_sync(bool value) {
		clock_sync_ = value;
	}

	const char *clock_sync_text() {
		return UiText::bool_to_on_off(clock_sync());
	}

	// Mode
	uint8_t mode() {
		return clock_sync_;
	}

	void set_mode(bool value) {
		mode_ = value;
	}

	const char *mode_text() {
		return mode_text(mode());
	}

	// Attack time
	uint8_t attack_time() {
		return attack_time_;
	}

	void set_attack_time(int value) {
		attack_time_ = stmlib::clip(0, 255, value);
	}

	const char *attack_time_text() {
		return nullptr;
	}

	float attack_inc() {
		return 0.f; //lut_phase_inc[attack_time()];
	}

	// Attack shape
	uint8_t attack_shape() {
		return attack_shape_;
	}

	void set_attack_shape(int value) {
		attack_shape_ = stmlib::clip(0, 255, value);
	}

	const char *attack_shape_text() {
		return nullptr;
	}

	// Decay time
	uint8_t decay_time() {
		return attack_time_;
	}

	void set_decay_time(int value) {
		attack_time_ = stmlib::clip(0, 255, value);
	}

	const char *decay_time_text() {
		return nullptr;
	}

	float decay_inc() {
		return 0.f; //lut_phase_inc[decay_time()];
	}

	// Decay shape
	uint8_t decay_shape() {
		return attack_shape_;
	}

	void set_decay_shape(int value) {
		attack_shape_ = stmlib::clip(0, 255, value);
	}

	const char *decay_shape_text() {
		return nullptr;
	}

	// Hold time
	uint8_t hold_time() {
		return hold_time_;
	}

	void set_hold_time(int value) {
		hold_time_ = stmlib::clip(0, 255, value);
	}

	const char *hold_time_text() {
		return nullptr;
	}

	float hold_inc() {
		return 0.f; //lut_phase_inc[sustain_time()];
	}


	// Sustain level
	float sustain_level() {
		return sustain_level_;
	}

	void set_sustain_level(int value) {
		sustain_level_ = stmlib::clip(0, 255, value);
	}

	const char *sustain_level_text() {
		return nullptr;
	}

	// Release time
	uint8_t release_time() {
		return release_time_;
	}

	void set_release_time(int value) {
		release_time_ = stmlib::clip(0, 255, value);
	}

	const char *release_time_text() {
		return nullptr;
	}

	float release_inc() {
		return 0.f; //lut_phase_inc[release_time()];
	}


	// Release shape
	uint8_t release_shape() {
		return release_shape_;
	}

	void set_release_shape(int value) {
		release_shape_ = stmlib::clip(0, 255, value);
	}

	const char *release_shape_text() {
		return nullptr;
	}

	// Storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(clock_sync_);
		fileWriter.write(mode_);
		fileWriter.write(attack_time_);
		fileWriter.write(attack_shape_);
		fileWriter.write(decay_time_);
		fileWriter.write(decay_shape_);
		fileWriter.write(hold_time_);
		fileWriter.write(sustain_level_);
		fileWriter.write(release_time_);
		fileWriter.write(release_shape_);
	}

	void load(FileReader &fileReader) {
		fileReader.read(clock_sync_);
		fileReader.read(mode_);
		fileReader.read(attack_time_);
		fileReader.read(attack_shape_);
		fileReader.read(decay_time_);
		fileReader.read(decay_shape_);
		fileReader.read(hold_time_);
		fileReader.read(sustain_level_);
		fileReader.read(release_time_);
		fileReader.read(release_shape_);
	}

private:
	bool clock_sync_;
	uint8_t mode_;
	uint8_t attack_time_;
	uint8_t attack_shape_;
	uint8_t decay_time_;
	uint8_t decay_shape_;
	uint8_t hold_time_;
	uint8_t sustain_level_;
	uint8_t release_time_;
	uint8_t release_shape_;
};

#endif
