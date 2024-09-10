#include "settings.h"

#include "diskUtils.h"

bool Settings::save(const char* new_path) {
	path.write(new_path, "/", project_name());
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
//	modulation().save(fileWriter);

//	for (size_t i = 0; i < kNumLfos; ++i) {
//		lfo(i).save(fileWriter);
//	}

//	for (size_t i = 0; i < kNumEnvelopes; ++i) {
//		envelope(i).save(fileWriter);
//	}

//	size_t num_samples_ = num_samples();
//	fileWriter.write(num_samples_);

//	for (size_t i = 0; i < num_samples_; ++i) {
//		sample(i)->save(fileWriter);
//	}

//	for (size_t i = 0; i < kNumInstruments; ++i) {
//		instrument(i).save(fileWriter);
//	}

	fileWriter.stop();
	MessagePainter::show(disk_->flags_text());

	//DiskUtils::print_result(fileWriter.last_res());


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

	// the wavImporter will assign the data to a free sample slot,
	// so we first load the sample into some temp memory so we dont overwrite it,
	// then we paste the temp memory into that assigned slot
	Sample sample;

	size_t num_samples_;
	fileReader.read(num_samples_);

	for (size_t i = 0; i < num_samples_; ++i) {
		sample.load(fileReader);

		if (wavImporter().import(sample.path(), sample.num_channels() == 1)) {
			int latest = sampleAllocator().num_samples() - 1;
			sampleAllocator().read_list(latest)->paste(&sample);
		}
	}

	for (size_t i = 0; i < kNumInstruments; ++i) {
		instrument(i).load(fileReader);
	}
	refresh_sample_selection();

	fileReader.stop();

	if (!fileReader.read_ok()) {
		init();
		return false;
	}

	path.write(new_path);	// the new valid path
	return true;
};
