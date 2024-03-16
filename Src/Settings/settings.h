#ifndef Settings_h
#define Settings_h

#include "sample.h"
#include "instrument.h"
#include "stringBuilder.h"

class Settings {

public:

	void init() {
		for (size_t i = 0; i < kMaxSamples; ++i) {
			sample(i).init();
		}

		for (size_t i = 0; i < kMaxInstruments; ++i) {
			instrument(i).init();
		}
	}

	Sample &sample(size_t index) {
		return sample_[index];
	}

	Instrument &instrument(size_t index) {
		return instrument_[index];
	}

	// save & load
	bool save();
	bool save(const char* new_path);
	bool load(const char* new_path);

	const char* read_path() {
		return path.read();
	}

	bool has_valid_path() {
		return path.length() > 0; // && StringUtils::is_equal(file_name.read(), song.name());
	}

	uint32_t current_version() {
		return 0;
	}

private:
	StringBuilderBase<63>path;

	static const size_t kMaxSamples = 128;
	static const size_t kMaxInstruments = 4;
	Sample sample_[kMaxSamples];
	Instrument instrument_[kMaxInstruments];

};

extern Settings settings;

#endif