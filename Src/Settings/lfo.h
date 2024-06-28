#ifndef Lfo_h
#define Lfo_h

#include "uiText.h"
#include "fileWriter.h"
#include "fileReader.h"
#include "midi.h"
#include "midiSync.h"

class Lfo {

public:

	void init() {
		set_speed(0);
		set_shape(0.5f);
		set_skew(0.5f);
		set_clock_sync(false);
		set_randomise(false);
		set_retrigger_port(Midi::UART);
		set_retrigger_channel(-1);
	}

	// speed
	float speed() {
		return speed_;
	}

	void set_speed(float value) {
		speed_ = stmlib::clip_float(value);
	}

	const char *speed_text() {
		if (clock_sync()) {
			return MidiSync::tempo_text(speed() * MidiSync::max_value());
		} else {
			return nullptr;
		}
	}

	float inc() {
		if (clock_sync()) {
			return MidiSync::read_inc(speed() * MidiSync::max_value());
		} else {
			return lut_phase_inc[int(speed() * (PHASE_TABLE_SIZE - 1))];
		}
	}

	// shape
	float shape() {
		return shape_;
	}

	void set_shape(float value) {
		shape_ = stmlib::clip_float(value);
	}

	const char *shape_text() {
		return UiText::float_to_text(shape(), -100, 100);
	}

	// Skew
	float skew() {
		return skew_;
	}

	void set_skew(float value) {
		skew_ = stmlib::clip_float(value);
	}

	const char *skew_text() {
		return UiText::float_to_text(skew(), -100, 100);
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

	// Retrigger port
	uint8_t retrigger_port() {
		return retrigger_port_;
	}

	void set_retrigger_port(uint8_t value) {
		retrigger_port_ = stmlib::clip(0, Midi::NUM_PORTS - 1, value);
	}

	const char *retrigger_port_text() {
		return Midi::port_text(retrigger_port());
	}

	bool retrigger_port_accepted(int port) {
		return port == retrigger_port();
	}


	// Retrigger channel
	int retrigger_channel() {
		return retrigger_channel_;
	}

	void set_retrigger_channel(int value) {
		retrigger_channel_ = stmlib::clip(-1, 16, value);
	}

	const char *retrigger_channel_text() {
		return UiText::midi_channel_text(retrigger_channel());
	}

	bool retrigger_channel_accepted(int channel) {
		return (retrigger_channel() >= 16) || (retrigger_channel() == channel);
	}

	// Storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(skew_);
		fileWriter.write(shape_);
		fileWriter.write(speed_);
		fileWriter.write(randomise_);
		fileWriter.write(clock_sync_);
		fileWriter.write(retrigger_port_);
		fileWriter.write(retrigger_channel_);
	}

	void load(FileReader &fileReader) {
		fileReader.read(skew_);
		fileReader.read(shape_);
		fileReader.read(speed_);
		fileReader.read(randomise_);
		fileReader.read(clock_sync_);
		fileReader.read(retrigger_port_);
		fileReader.read(retrigger_channel_);
	}

	void paste(Lfo *lfo) {
		skew_ = lfo->skew();
		shape_ = lfo->shape();
		speed_ = lfo->speed();
		randomise_ = lfo->randomise();
		clock_sync_ = lfo->clock_sync();
		retrigger_port_ = lfo->retrigger_port();
		retrigger_channel_ = lfo->retrigger_channel();
	}

private:
	float skew_;
	float shape_;
	float speed_;
	bool randomise_;
	bool clock_sync_;
	uint8_t retrigger_port_;
	int retrigger_channel_;
};

#endif
