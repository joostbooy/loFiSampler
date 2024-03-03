#ifndef Sample_h
#define Sample_h

#include "uiText.h"
#include "stringUtils.h"
#include "fileWriter.h"
#include "fileReader.h"

class Sample {

public:

	void init() {
		set_data(nullptr);

		//	set_size(0);
		//	set_start(0);


		set_loop(false);
		set_root_note(60);
		set_key_range_low(0);
		set_key_range_high(127);

		set_name(" ");
	}

	// data
	void set_data(uint16_t* value) {
		data_ = value;
	}

	uint16_t* data() {
		return data_;
	}

	bool has_data() {
		return data_ != nullptr;
	}

	// size
	void set_size(size_t value) {
		size_ = value;
	}

	size_t size() {
		return size_;
	}

	const char *size_text() {
		return UiText::str.write(size());
	}

	// start
	void set_start(size_t value) {
		start_ = stmlib::clip(0, size(), value);
	}

	size_t start() {
		return start_;
	}

	const char *start_text() {
		return UiText::str.write(start());
	}

	// end
	void set_end(size_t value) {
		end_ = stmlib::clip(start(), size(), value);
	}

	size_t end() {
		return end_;
	}

	const char *end_text() {
		return UiText::str.write(end());
	}

	// loop start
	void set_loop_start(size_t value) {
		loop_start_ = stmlib::clip(0, loop_end(), value);
	}

	size_t loop_start() {
		return loop_start_;
	}

	const char *loop_start_text() {
		return UiText::str.write(loop_start());
	}

	// loop end
	void set_loop_end(size_t value) {
		loop_end_ = stmlib::clip(loop_start(), 0xFFFFFFFF, value);
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

	// name
	const char *name() {
		return name_;
	}

	void set_name(const char *value) {
		return StringUtils::copy(name_, const_cast<char*>(value), max_name_length());
	}

	const size_t max_name_length() {
		return kMaxNameLength;
	}

	// Storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(size_);
		fileWriter.write(size_);
		fileWriter.write(size_);
		fileWriter.write(size_);
	}

	void load(FileReader &fileReader) {
		fileReader.read(size_);
		fileReader.read(size_);
		fileReader.read(size_);
		fileReader.read(size_);
	}

private:
	uint16_t *data_;
	size_t size_;
	size_t start_;
	size_t end_;
	bool loop_;
	size_t loop_start_;
	size_t loop_end_;
	uint8_t root_note_;
	uint8_t key_range_low_;
	uint8_t key_range_high_;

	static const size_t kMaxNameLength = 8;
	char name_[kMaxNameLength];
};

#endif
