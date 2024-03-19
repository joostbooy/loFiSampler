#ifndef Settings_h
#define Settings_h

#include "lfo.h"
#include "midi.h"
#include "sample.h"
#include "modulation.h"
#include "instrument.h"
#include "envelope.h"
#include "stringBuilder.h"

class Settings {

public:

	constexpr static const size_t num_lfos() {
		return kNumLfos;
	}

	constexpr static const size_t num_envelopes() {
		return kNumLfos;
	}

	void init() {
		selected_sample_ = 0;
		selected_instrument_ = 0;

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

		for (size_t i = 0; i < kMaxInstruments; ++i) {
			instrument(i).init();
		}
	}

	Midi &midi() {
		return midi_;
	}

	Modulation &modulation() {
		return modulation_;
	}

	Sample &sample(size_t index) {
		return sample_[index];
	}

	Instrument &instrument(size_t index) {
		return instrument_[index];
	}

	Lfo &lfo(size_t index) {
		return lfo_[index];
	}

	Envelope &envelope(size_t index) {
		return envelope_[index];
	}

	int selected_sample() {
		return selected_sample_;
	}

	void set_selected_sample(int index) {
		selected_sample_ = stmlib::clip(0, instrument(0).num_samples() - 1, index);
	}

	int selected_instrument() {
		return selected_instrument_;
	}

	void set_selected_instrument(int index) {
		selected_instrument_ = stmlib::clip(0, kMaxInstruments - 1, index);
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
	static const size_t kNumLfos = 4;
	static const size_t kNumEnvelopes = 2;

	Midi midi_;
	Modulation modulation_;
	Sample sample_[kMaxSamples];
	Instrument instrument_[kMaxInstruments];
	Lfo lfo_[kNumLfos];
	Envelope envelope_[kNumEnvelopes];

	int selected_sample_;
	int selected_instrument_;
};

extern Settings settings;

#endif
