#include "settings.h"

bool Settings::save(const char* new_path) {
	path.write(new_path);
	if (!save()) {
		path.clear();
		return false;
	}
	return true;
}

bool Settings::save() {
	if (!path.length()) {
		return false;
	}

	fileWriter.start(&disk_->file(), path.read(), current_version());

	midi().save(fileWriter);
	modulation().save(fileWriter);

	for (size_t i = 0; i < kNumLfos; ++i) {
		lfo(i).save(fileWriter);
	}

	for (size_t i = 0; i < kNumEnvelopes; ++i) {
		envelope(i).save(fileWriter);
	}

	for (size_t i = 0; i < kMaxSamples; ++i) {
		sample(i).save(fileWriter);
	}

	for (size_t i = 0; i < kNumInstruments; ++i) {
		instrument(i).save(fileWriter);
	}

	fileWriter.stop();

	if (!fileWriter.write_ok()) {
		return false;
	}

	return true;
};

bool Settings::load(const char* new_path) {
	init();	// also clears the path

	fileReader.start(&disk_->file(), new_path);

	midi().load(fileReader);
	modulation().load(fileReader);

	for (size_t i = 0; i < kNumLfos; ++i) {
		lfo(i).load(fileReader);
	}

	for (size_t i = 0; i < kNumEnvelopes; ++i) {
		envelope(i).load(fileReader);
	}

	for (size_t i = 0; i < kMaxSamples; ++i) {
		sample(i).load(fileReader);
	}

	for (size_t i = 0; i < kNumInstruments; ++i) {
		instrument(i).load(fileReader);
	}

	fileReader.stop();

	if (!fileReader.read_ok()) {
		init();
		return false;
	}

	path.write(new_path);	// the new valid path
	return true;
};
