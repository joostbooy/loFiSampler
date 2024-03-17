#ifndef Sample_h
#define Sample_h

#include "uiText.h"
#include "sampleData.h"
#include "stringUtils.h"
#include "fileWriter.h"
#include "fileReader.h"

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
		case FORWARD:				return "FORWARD";
		case BACKWARD:				return "BACKWARD";
		default:
			break;
		}
		return nullptr;
	}

	void init() {
		// set_data(nullptr);

		//	set_size(0);
		//	set_start(0);

		set_loop(false);
		set_u_turn(false);
		set_play_mode(FORWARD);
		set_root_note(60);
		set_key_range_low(0);
		set_key_range_high(127);
	}

	// data
	void set_data(SampleData::Entry *entry) {
		entry_ = entry;
	}

	int16_t* data() {
		return entry_->data;
	}

	bool has_data() {
		return entry_ != nullptr;
	}

	// size
	void set_size(size_t value) {
		//	size_ = value;
	}

	size_t size() {
		return entry_->size;
	}

	const char *size_text() {
		return UiText::str.write(size());
	}

	// start
	void set_start(size_t value) {
		start_ = stmlib::clip(0, end() - 1, value);
	}

	size_t start() {
		return start_;
	}

	const char *start_text() {
		return UiText::str.write(start());
	}

	// end
	void set_end(size_t value) {
		end_ = stmlib::clip(start() + 1, size(), value);
	}

	size_t end() {
		return end_;
	}

	const char *end_text() {
		return UiText::str.write(end());
	}

	// loop start
	void set_loop_start(size_t value) {
		loop_start_ = stmlib::clip(1, loop_end(), value);
	}

	size_t loop_start() {
		return loop_start_;
	}

	const char *loop_start_text() {
		return UiText::str.write(loop_start());
	}

	// loop end
	void set_loop_end(size_t value) {
		loop_end_ = stmlib::clip(loop_start(), size() - 1, value);
	}

	size_t loop_end() {
		return loop_end_;
	}

	const char *loop_end_text() {
		return UiText::str.write(loop_end());
	}

	// loop
	void set_loop(bool value) {
		loop_ = value;
	}

	bool loop() {
		return loop_;
	}

	const char *loop_text() {
		return UiText::bool_to_on_off(loop());
	}

	// Root note
	void set_root_note(uint8_t value) {
		root_note_ = stmlib::clip(0, 127, value);
	}

	uint8_t root_note() {
		return root_note_;
	}

	const char *root_note_text() {
		return UiText::note_to_text(root_note());
	}

	// Key range low
	void set_key_range_low(uint8_t value) {
		key_range_low_ = stmlib::clip(0, key_range_high(), value);
	}

	uint8_t key_range_low() {
		return key_range_low_;
	}

	const char *key_range_low_text() {
		return UiText::note_to_text(key_range_low());
	}

	// Key range high
	void set_key_range_high(uint8_t value) {
		key_range_low_ = stmlib::clip(key_range_low(), 127, value);
	}

	uint8_t key_range_high() {
		return key_range_high_;
	}

	const char *key_range_high_text() {
		return UiText::note_to_text(key_range_high());
	}

	bool within_key_range(uint8_t key) {
		return key >= key_range_low() && key <= key_range_high();
	}

	// Play mode
	void set_play_mode(uint8_t value) {
		play_mode_ = stmlib::clip(0, NUM_PLAY_MODES - 1, value);
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
		return UiText::bool_to_on_off(u_turn());
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
		fileWriter.write(root_note_);
		fileWriter.write(key_range_low_);
		fileWriter.write(key_range_high_);
		fileWriter.write(bit_depth_);
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
		fileReader.read(root_note_);
		fileReader.read(key_range_low_);
		fileReader.read(key_range_high_);
		fileReader.read(bit_depth_);
	}

private:
	SampleData::Entry *entry_;
	size_t start_;
	size_t end_;
	bool loop_;
	bool u_turn_;
	uint8_t play_mode_;
	size_t loop_start_;
	size_t loop_end_;
	uint8_t root_note_;
	uint8_t key_range_low_;
	uint8_t key_range_high_;
	uint8_t bit_depth_;
};

#endif
