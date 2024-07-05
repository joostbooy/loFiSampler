#ifndef Settings_h
#define Settings_h

#include "lfo.h"
#include "midi.h"
#include "sample.h"
#include "modulation.h"
#include "instrument.h"
#include "envelope.h"
#include "stringBuilder.h"
#include "settingsUtils.h"

class Settings {

public:

	static const size_t kMaxVoices = 8;
	static const size_t kMaxSamples = 128;
	static const size_t kNumInstruments = 8;
	static const size_t kNumLfos = 4;
	static const size_t kNumEnvelopes = 2;

	void init() {
		selected_sample_index_ = 0;
		selected_lfo_index_ = 0;
		selected_instrument_index_ = 0;
		selected_envelope_index_ = 0;

		path.clear();

		midi().init();
		modulation().init();

		for (size_t i = 0; i < kNumLfos; ++i) {
			lfo(i).init();
		}

		for (size_t i = 0; i < kNumEnvelopes; ++i) {
			envelope(i).init();
		}

		for (size_t i = 0; i < kMaxSamples; ++i) {
			sample(i).init();
		}

		for (size_t i = 0; i < kNumInstruments; ++i) {
			instrument(i).init();
		}

		ModulationMatrix::init(&modulation_);
	}

	Midi &midi() {
		return midi_;
	}

	Modulation &modulation() {
		return modulation_;
	}

	// Sample
	Sample &sample(size_t index) {
		return sample_[index];
	}

	int selected_sample_index() {
		return selected_sample_index_;
	}

	void select_sample_index(int index) {
		int num_samples = selected_instrument().num_samples();
		if (num_samples > 0) {
			selected_sample_index_ = SettingsUtils::clip(0, num_samples - 1, index);
		} else {
			selected_sample_index_ = 0;
		}
	}

	Sample &selected_sample() {
		return sample_[selected_sample_index_];
	}

	// Envelope
	Envelope &envelope(size_t index) {
		return envelope_[index];
	}

	int selected_envelope_index() {
		return selected_envelope_index_;
	}

	void select_envelope_index(int index) {
		selected_envelope_index_ = SettingsUtils::clip(0, kNumEnvelopes - 1, index);
	}

	Envelope &selected_envelope() {
		return envelope_[selected_envelope_index_];
	}

	// Lfo
	Lfo &lfo(size_t index) {
		return lfo_[index];
	}

	int selected_lfo_index() {
		return selected_lfo_index_;
	}

	void select_lfo_index(int index) {
		selected_lfo_index_ = SettingsUtils::clip(0, kNumLfos - 1, index);
	}

	Lfo &selected_lfo() {
		return lfo_[selected_lfo_index_];
	}

	// Instrument
	Instrument &instrument(size_t index) {
		return instrument_[index];
	}

	int selected_instrument_index() {
		return selected_instrument_index_;
	}

	void select_instrument_index(int index) {
		selected_instrument_index_ = SettingsUtils::clip(0, kNumInstruments - 1, index);
		select_sample_index(selected_sample_index_);
	}

	Instrument &selected_instrument() {
		return instrument_[selected_instrument_index_];
	}

	ModulationMatrix &selected_modulation_matrix() {
		return instrument_[selected_instrument_index_].modulationMatrix();
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

	Midi midi_;
	Modulation modulation_;
	Lfo lfo_[kNumLfos];
	Sample sample_[kMaxSamples];
	Envelope envelope_[kNumEnvelopes];
	Instrument instrument_[kNumInstruments];

	FileWriter fileWriter;
	FileReader fileReader;

	int selected_lfo_index_;
	int selected_sample_index_;
	int selected_instrument_index_;
	int selected_envelope_index_;
};

//extern Settings settings;

#endif
