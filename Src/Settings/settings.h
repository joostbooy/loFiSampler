#ifndef Settings_h
#define Settings_h

#include "lfo.h"
#include "midi.h"
#include "sample.h"
#include "modulation.h"
#include "instrument.h"
#include "envelope.h"
#include "delay.h"
#include "stringBuilder.h"
#include "settingsUtils.h"
#include "disk.h"
#include "wavImporter.h"

class Settings {

public:

	static const size_t kMaxVoices = 8;
	static const size_t kNumInstruments = 8;
	static const size_t kNumLfos = 4;
	static const size_t kNumEnvelopes = 2;

	void init(Sdram *sdram, Disk *disk) {
		sdram_ = sdram;
		disk_ = disk;

		selected_sample_index_ = 0;
		selected_lfo_index_ = 0;
		selected_instrument_index_ = 0;
		selected_envelope_index_ = 0;
		selected_instrument_sample_index_ = 0;

		path.clear();

		midi().init();
		delay().init();
		modulation().init();
		sampleAllocator().init(sdram);
		wavImporter().init(&sampleAllocator_);
		Instrument::init(&sampleAllocator_);
		ModulationMatrix::init(&modulation_);

		for (size_t i = 0; i < kNumLfos; ++i) {
			lfo(i).init();
		}

		for (size_t i = 0; i < kNumEnvelopes; ++i) {
			envelope(i).init();
		}

		for (size_t i = 0; i < kNumInstruments; ++i) {
			instrument(i).init();
		}

		set_project_name("NEW_PRJ");
	}

	void init() {
		if (sdram_ != nullptr && disk_ != nullptr) {
			init(sdram_, disk_);
		}
	}

	Midi &midi() {
		return midi_;
	}

	Delay &delay() {
		return delay_;
	}

	Modulation &modulation() {
		return modulation_;
	}

	// Sample
	Sample *sample(size_t index) {
		return sampleAllocator_.read_list(index);
	}

	int selected_sample_index() {
		return selected_sample_index_;
	}

	Sample *selected_sample() {
		return sample(selected_sample_index_);
	}

	void select_sample_index(int index) {
		int num_samples = sampleAllocator_.num_samples();
		if (num_samples > 0) {
			selected_sample_index_ = SettingsUtils::clip(0, num_samples - 1, index);
		} else {
			selected_sample_index_ = 0;
		}
	}

	size_t num_samples() {
		return sampleAllocator_.num_samples();
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

	int selected_instrument_sample_index() {
		return selected_instrument_sample_index_;
	}

	void select_instrument_index(int index) {
		selected_instrument_index_ = SettingsUtils::clip(0, kNumInstruments - 1, index);
		select_instrument_sample_index(selected_instrument_sample_index_);
	}

	void select_instrument_sample_index(int index) {
		int num_samples = selected_instrument().num_samples();
		if (num_samples > 0) {
			selected_instrument_sample_index_ = SettingsUtils::clip(0, num_samples - 1, index);
		} else {
			selected_instrument_sample_index_ = 0;
		}
	}

	Instrument &selected_instrument() {
		return instrument_[selected_instrument_index_];
	}

	Sample *selected_instrument_sample() {
		return selected_instrument().sample(selected_instrument_sample_index_);
	}

	ModulationMatrix &selected_modulation_matrix() {
		return instrument_[selected_instrument_index_].modulationMatrix();
	}

	void refresh_sample_selection() {
		// needs to be first
		for (size_t i = 0; i < kNumInstruments; ++i) {
			instrument(i).refresh_sample_list();
		}

		select_instrument_sample_index(selected_instrument_sample_index_);
		select_sample_index(selected_sample_index_);
	}

	// save & load
	bool save();
	bool save(const char* new_path);
	bool load(const char* new_path);

	bool has_valid_path() {
		char file_name[max_name_length()];

		if (path.length() > 0) {
			StringUtils::get_file_name_from_path(const_cast<char*>(path.read()), file_name);
			StringUtils::remove_extension(file_name);
			return StringUtils::text_is_equal(file_name, project_name_);
		}
		return false;
	}

	uint32_t current_version() {
		return 0;
	}

	Sdram *sdram() {
		return sdram_;
	}

	Disk *disk() {
		return disk_;
	}

	WavImporter &wavImporter() {
		return wavImporter_;
	}

	SampleAllocator &sampleAllocator() {
		return sampleAllocator_;
	}

	// name
	const char *project_name() {
		return project_name_;
	}

	void set_project_name(const char *value) {
		return StringUtils::copy(project_name_, const_cast<char*>(value), max_name_length());
	}

	const size_t max_name_length() {
		return 8;
	}


private:
	StringBuilderBase<63>path;

	Midi midi_;
	Delay delay_;
	Modulation modulation_;
	Lfo lfo_[kNumLfos];
	Envelope envelope_[kNumEnvelopes];
	Instrument instrument_[kNumInstruments];

	WavImporter wavImporter_;
	SampleAllocator sampleAllocator_;

	FileWriter fileWriter;
	FileReader fileReader;

	Sdram *sdram_ = nullptr;
	Disk *disk_ = nullptr;

	int selected_lfo_index_;
	int selected_sample_index_;
	int selected_instrument_index_;
	int selected_envelope_index_;
	int selected_instrument_sample_index_;

	char project_name_[8];
};

#endif
