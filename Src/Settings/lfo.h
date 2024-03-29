#ifndef Lfo_h
#define Lfo_h

#include "uiText.h"
#include "fileWriter.h"
#include "fileReader.h"
#include "midi.h"

class Lfo {

public:

	void init() {
		set_speed(0);
		set_shape(32);
		set_skew(32);
		set_clock_sync(false);
		set_randomise(false);
	}

	// speed
	uint8_t speed() {
		return speed_;
	}

	void set_speed(int value) {
		speed_ = stmlib::clip(0, 255, value);
	}

	const char *speed_text() {
		return nullptr;
	}

	float inc() {
		return clock_sync() ? Midi::read_tempo_phase_inc(0, 0, 0) : 0.f;
	}

	// shape
	int8_t shape() {
		return shape_;
	}

	void set_shape(int value) {
		shape_ = stmlib::clip(0, 64, value);
	}

	const char *shape_text() {
		return UiText::str.write(skew() - 32);
	}

	float shape_float() {
		return (1.f / 64.f) * shape();
	}

	// Skew
	int8_t skew() {
		return skew_;
	}

	void set_skew(int value) {
		skew_ = stmlib::clip(0, 64, value);
	}

	const char *skew_text() {
		return UiText::str.write(skew() - 32);
	}

	float skew_float() {
		return (1.f / 64.f) * skew();
	}

	// clock sync
	bool clock_sync() {
		return clock_sync_;
	}

	void set_clock_sync(bool value) {
		clock_sync_ = value;
	}

	const char *clock_sync_text() {
		return UiText::bool_to_on_off(clock_sync());
	}

	// Randomise
	bool randomise() {
		return randomise_;
	}

	void set_randomise(bool value) {
		randomise_ = value;
	}

	const char *randomise_text() {
		return UiText::bool_to_on_off(clock_sync());
	}

	// Storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(skew_);
		fileWriter.write(shape_);
		fileWriter.write(speed_);
		fileWriter.write(randomise_);
		fileWriter.write(clock_sync_);
	}

	void load(FileReader &fileReader) {
		fileReader.read(skew_);
		fileReader.read(shape_);
		fileReader.read(speed_);
		fileReader.read(randomise_);
		fileReader.read(clock_sync_);
	}

private:
	int8_t skew_;
	int8_t shape_;
	uint8_t speed_;
	bool randomise_;
	bool clock_sync_;
};

#endif
