#ifndef Sample_h
#define Sample_h

#include "settingsText.h"
#include "settingsUtils.h"
#include "sampleData.h"
#include "stringUtils.h"
#include "fileWriter.h"
#include "fileReader.h"
#include "sdram.h"

class Sample {

public:

	enum PlayMode {
		FORWARD,
		BACKWARD,

		NUM_PLAY_MODES
	};

	static const char* play_mode_text(int mode) {
		switch (mode)
		{
		case FORWARD:	return "FORWARD";
		case BACKWARD:	return "BACKWARD";
		default:
			break;
		}
		return nullptr;
	}

	static void init(Sdram *sdram) {
		sdram_ = sdram;
	}

	void init() {
		// set_data(nullptr);

		set_loop(false);
		set_u_turn(false);
		set_play_mode(FORWARD);
		set_cents(0);
		set_root_note(60);
		set_gain(1.0);
		set_pan(0.5f);

		// these need to be initialised without setters,
		// because they depend on eachother
		start_ = 0;
		end_ = 1;
		loop_start_ = 0;
		loop_end_ = 1;
		key_range_low_ = 0;
		key_range_high_ = 127;
	}

	// data
	void set_data(SampleData::Entry *entry) {
		entry_ = entry;
	}

	void read(size_t index, int16_t *left, int16_t *right) {
		if (is_stereo()) {
			size_t i = index * 2;
			*left = entry_->data[i];
			*right = entry_->data[i + 1];
		} else {
			*left = entry_->data[index];
			*right = *left;
		}
	}

	bool has_data() {
		return entry_ != nullptr;
	}

	// size
	size_t size() {
		return is_stereo() ? entry_->size / 2 : entry_->size;
	}

	const char *size_text() {
		return SettingsText::samples_to_time(size());
	}


	// channels
	int num_channels() {
		return entry_->num_channels;
	}

	bool is_stereo() {
		return entry_->num_channels == 2;
	}


	// start
	void set_start(size_t value) {
		start_ = SettingsUtils::clip(0, end() - 1, value);
	}

	size_t start() {
		return start_;
	}

	const char *start_text() {
		return SettingsText::samples_to_time(start());
	}

	// end
	void set_end(size_t value) {
		end_ = SettingsUtils::clip(start() + 1, size(), value);
	}

	size_t end() {
		return end_;
	}

	const char *end_text() {
		return SettingsText::samples_to_time(end());
	}

	// loop start
	void set_loop_start(size_t value) {
		loop_start_ = SettingsUtils::clip(1, loop_end(), value);
	}

	size_t loop_start() {
		return loop_start_;
	}

	const char *loop_start_text() {
		return SettingsText::samples_to_time(loop_start());
	}

	// loop end
	void set_loop_end(size_t value) {
		loop_end_ = SettingsUtils::clip(loop_start(), size() - 1, value);
	}

	size_t loop_end() {
		return loop_end_;
	}

	const char *loop_end_text() {
		return SettingsText::samples_to_time(loop_end());
	}

	// loop
	void set_loop(bool value) {
		loop_ = value;
	}

	bool loop() {
		return loop_;
	}

	const char *loop_text() {
		return SettingsText::bool_to_on_off(loop());
	}

	// Cents
	void set_cents(int value) {
		cents_ = SettingsUtils::clip(-99, 99, value);
	}

	int cents() {
		return cents_;
	}

	const char *cents_text() {
		return SettingsText::str.write(cents());
	}

	// Root note
	void set_root_note(uint8_t value) {
		root_note_ = SettingsUtils::clip(0, 127, value);
	}

	uint8_t root_note() {
		return root_note_;
	}

	const char *root_note_text() {
		return SettingsText::note_to_text(root_note());
	}

	// Key range low
	void set_key_range_low(uint8_t value) {
		key_range_low_ = SettingsUtils::clip(0, key_range_high(), value);
	}

	uint8_t key_range_low() {
		return key_range_low_;
	}

	const char *key_range_low_text() {
		return SettingsText::note_to_text(key_range_low());
	}

	// Key range high
	void set_key_range_high(uint8_t value) {
		key_range_low_ = SettingsUtils::clip(key_range_low(), 127, value);
	}

	uint8_t key_range_high() {
		return key_range_high_;
	}

	const char *key_range_high_text() {
		return SettingsText::note_to_text(key_range_high());
	}

	bool within_key_range(uint8_t key) {
		return key >= key_range_low() && key <= key_range_high();
	}

	// Play mode
	void set_play_mode(uint8_t value) {
		play_mode_ = SettingsUtils::clip(0, NUM_PLAY_MODES - 1, value);
	}

	uint8_t play_mode() {
		return play_mode_;
	}

	const char *play_mode_text() {
		return play_mode_text(play_mode());
	}

	// U turn
	void set_u_turn(bool value) {
		u_turn_ = value;
	}

	bool u_turn() {
		return u_turn_;
	}

	const char *u_turn_text() {
		return SettingsText::bool_to_on_off(u_turn());
	}

	// Gain
	void set_gain(float value) {
		gain_ = SettingsUtils::clip_float(value);
	}

	float gain() {
		return gain_;
	}

	const char *gain_text() {
		return SettingsText::float_to_text(0, 100, gain());
	}

	// pan
	float pan() {
		return pan_;
	}

	void set_pan(float value) {
		pan_ = SettingsUtils::clip_float(value);
	}

	const char* pan_text() {
		return SettingsText::float_to_text(-100, 100, pan());
	}


	// name to note
	bool map_name_to_root_note(const char *name) {
		uint8_t note;
		if (name_to_midi_note(name, &note)) {
			root_note_ = note;
			return true;
		}
		return false;
	}

	// Storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(entry_);
		fileWriter.write(start_);
		fileWriter.write(end_);
		fileWriter.write(loop_);
		fileWriter.write(u_turn_);
		fileWriter.write(play_mode_);
		fileWriter.write(loop_start_);
		fileWriter.write(loop_end_);
		fileWriter.write(cents_);
		fileWriter.write(root_note_);
		fileWriter.write(key_range_low_);
		fileWriter.write(key_range_high_);
		fileWriter.write(gain_);
		fileWriter.write(pan_);
	}

	void load(FileReader &fileReader) {
		fileReader.read(entry_);
		fileReader.read(start_);
		fileReader.read(end_);
		fileReader.read(loop_);
		fileReader.read(u_turn_);
		fileReader.read(play_mode_);
		fileReader.read(loop_start_);
		fileReader.read(loop_end_);
		fileReader.read(cents_);
		fileReader.read(root_note_);
		fileReader.read(key_range_low_);
		fileReader.read(key_range_high_);
		fileReader.read(gain_);
		fileReader.read(pan_);
	}

	void paste(Sample *sample) {
		start_ = sample->start();
		end_ = sample->end();
		loop_ = sample->loop();
		u_turn_ = sample->u_turn();
		play_mode_ = sample->play_mode();
		loop_start_ = sample->loop_start();
		loop_end_ = sample->loop_end();
		cents_ = sample->cents();
		root_note_ = sample->root_note();
		key_range_low_ = sample->key_range_low();
		key_range_high_ = sample->key_range_high();
		gain_ = sample->gain();
		pan_ = sample->gain();
	}

private:
	static Sdram *sdram_;

	SampleData::Entry *entry_;
	size_t start_;
	size_t end_;
	bool loop_;
	bool u_turn_;
	uint8_t play_mode_;
	size_t loop_start_;
	size_t loop_end_;
	int cents_;
	uint8_t root_note_;
	uint8_t key_range_low_;
	uint8_t key_range_high_;
	float gain_;
	float pan_;

	static bool name_to_midi_note(const char *name, uint8_t *note);
	static constexpr const uint8_t char_to_note_[7] = { 9, 11, 0, 2, 4, 5, 7 };
	static constexpr const bool has_seminote_[12] = { 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0 };
};

#endif
