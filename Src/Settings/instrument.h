#ifndef Instrument_h
#define Instrument_h

#include "settingsText.h"
#include "settingsUtils.h"
#include "dac.h"
#include "fileWriter.h"
#include "fileReader.h"
#include "midiEngine.h"
#include "sample.h"
#include "modulationMatrix.h"
#include "sampleAllocator.h"

class Instrument {

public:

	class SampleList;
	static const size_t kMaxNameLength = 16;
	static const size_t kMaxNumSamples = 128;

	static void init(SampleAllocator *sampleAllocator) {
		sampleAllocator_ = sampleAllocator;
	}

	void init() {
		set_name("NEW INSTRUMENT");
		set_gain(1.0);
		set_pan(0.5);
		set_audio_channel(0);
		set_midi_channel(16);
		set_midi_port(Midi::UART);
		set_bit_depth(16);
		set_bend_range(2);
		set_bend(0.5);
		set_sample_rate_divider(1);

		modulationMatrix().init();

		clear_samples();
	}

	// modulationMatrix
	ModulationMatrix& modulationMatrix() {
		return modulationMatrix_;
	}

	// Sample
	SampleList &sampleList(size_t index) {
		return sampleList_[index];
	}

	Sample *sample(size_t index) {
		return sampleList_[index].sample;
	}

	size_t num_samples() {
		return num_samples_;
	}

	void clear_samples() {
		num_samples_ = 0;
	}

	bool add_sample(Sample *sample) {
		if (num_samples_ < kMaxNumSamples && sample != nullptr && sample_in_list(sample) == false) {
			sampleList_[num_samples_].sample = sample;
			sampleList_[num_samples_].hash = sample->hash();
			++num_samples_;
			return true;
		}
		return false;
	}

	bool remove_sample(size_t index) {
		if (index >= num_samples_) {
			return false;
		}

		for (size_t i = 0; i < index; ++i) {
			sampleList_[i] = sampleList_[i + 1];
		}
		--num_samples_;

		return true;
	}

	void refresh_sample_list() {
		size_t index = 0;
		while (index < num_samples_) {
			SampleList &s = sampleList_[index];

			if (s.hash == s.sample->hash() && s.sample->has_data() == true) {
				++index;
			} else {
				Sample *sample = hash_to_sample(s.hash);
				if (sample) {
					s.sample = sample;
					++index;
				} else {
					remove_sample(index);
				}
			}
		}
	}

	// pan
	float pan() {
		return pan_;
	}

	void set_pan(float value) {
		pan_ = SettingsUtils::clip_float(value);
	}

	const char* pan_text() {
		return SettingsText::float_to_text(pan(), -100, 100);
	}

	// Midi channel
	int8_t midi_channel() {
		return midi_channel_;
	}

	void set_midi_channel(int8_t value) {
		midi_channel_ = SettingsUtils::clip(0, 16, value);
	}

	const char* midi_channel_text() {
		return SettingsText::midi_channel_text(midi_channel());
	}

	bool midi_channel_accepted(int channel) {
		return midi_channel() >= 16 || channel == midi_channel();
	}

	// Midi port
	int8_t midi_port() {
		return midi_port_;
	}

	void set_midi_port(int8_t value) {
		midi_port_ = SettingsUtils::clip(0, Midi::NUM_PORTS - 1, value);
	}

	const char* midi_port_text() {
		return nullptr;
	}

	bool midi_port_accepted(int port) {
		return midi_port() == port;
	}

	bool midi_accepted(MidiEngine::Event &e) {
		return midi_port_accepted(e.port) && midi_channel_accepted(e.message & 0x0F);
	}

	// Bit depth
	void set_bit_depth(int value) {
		bit_depth_ = SettingsUtils::clip(2, 16, value);
	}

	uint8_t bit_depth() {
		return bit_depth_;
	}

	const char *bit_depth_text() {
		return SettingsText::str.write(bit_depth(), " BIT");
	}

	// Sample rate divider
	int sample_rate_divider() {
		return sample_rate_divider_;
	}

	void set_sample_rate_divider(int value) {
		sample_rate_divider_ = SettingsUtils::clip(1, 125, value);
	}

	const char* sample_rate_divider_text() {
		return SettingsText::str.write(SAMPLE_RATE / sample_rate_divider(), " HZ");
	}

	// Audio channel
	int8_t audio_channel() {
		return audio_channel_;
	}

	void set_audio_channel(int8_t value) {
		audio_channel_ = SettingsUtils::clip(0, Dac::kNumStereoChannels - 1, value);
	}

	const char* audio_channel_text() {
		return SettingsText::audio_channel_to_text(audio_channel());
	}

	// Bend range
	uint8_t bend_range() {
		return bend_range_;
	}

	void set_bend_range(int value) {
		bend_range_ = SettingsUtils::clip(0, 24, value);
	}

	const char* bend_range_text() {
		return SettingsText::str.write(bend_range(), " SEMITONES");
	}

	// Bend
	float bend() {
		return bend_;
	}

	void set_bend(float value) {
		bend_ = SettingsUtils::clip_float(value);
	}

	const char* bend_text() {
		return SettingsText::str.write(Dsp::cross_fade(-1, 1, bend()));
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

	// Gain
	float gain() {
		return gain_;
	}

	void set_gain(float value) {
		gain_ = SettingsUtils::clip_float(value);
	}

	const char *gain_text() {
		return SettingsText::percentage_to_text(gain() * 100, 100);
	}


	// Storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(pan_);
		fileWriter.write(gain_);
		fileWriter.write(bend_);
		fileWriter.write(audio_channel_);
		fileWriter.write(midi_port_);
		fileWriter.write(midi_channel_);
		fileWriter.write(bit_depth_);
		fileWriter.write(bend_range_);
		fileWriter.write(num_samples_);
		fileWriter.write(sample_rate_divider_);

		fileWriter.write(num_samples_);
		for (size_t i = 0; i < kMaxNumSamples; ++i) {
			fileWriter.write(sampleList_[i]);
		}

		modulationMatrix().save(fileWriter);
	}

	void load(FileReader &fileReader) {
		fileReader.read(pan_);
		fileReader.read(gain_);
		fileReader.read(bend_);
		fileReader.read(audio_channel_);
		fileReader.read(midi_port_);
		fileReader.read(midi_channel_);
		fileReader.read(bit_depth_);
		fileReader.read(bend_range_);
		fileReader.read(num_samples_);
		fileReader.read(sample_rate_divider_);

		fileReader.read(num_samples_);
		for (size_t i = 0; i < kMaxNumSamples; ++i) {
			fileReader.read(sampleList_[i]);
		}

		modulationMatrix().load(fileReader);
	}

	void paste(Instrument *instrument) {
		pan_ = instrument->pan();
		gain_ = instrument->gain();
		bend_ = instrument->bend();
		audio_channel_ = instrument->audio_channel();
		midi_port_ = instrument->midi_port();
		midi_channel_ = instrument->midi_channel();
		bit_depth_ = instrument->bit_depth();
		bend_range_ = instrument->bend_range();
		bit_depth_ = instrument->bit_depth();
		sample_rate_divider_ = instrument->sample_rate_divider();
	}

	void paste_sample_list(Instrument *instrument) {
		num_samples_ = instrument->num_samples();
		for (size_t i = 0; i < kMaxNumSamples; ++i) {
			sampleList_[i] = instrument->sampleList(i);
		}
	}

private:
	float pan_;
	float gain_;
	float bend_;
	int8_t audio_channel_;
	int8_t midi_port_;
	int8_t midi_channel_;
	int8_t bit_depth_;
	int sample_rate_divider_;
	uint8_t bend_range_;
	size_t num_samples_;
	char name_[kMaxNameLength];

	ModulationMatrix modulationMatrix_;
	static SampleAllocator *sampleAllocator_;

	struct SampleList {
		Sample *sample;
		uint32_t hash;
	}sampleList_[kMaxNumSamples];

	Sample* hash_to_sample(uint32_t hash) {
		for (size_t i = 0; i < sampleAllocator_->num_samples(); ++i) {
			Sample *sample = sampleAllocator_->read_list(i);
			if (hash == sample->hash()) {
				return sample;
			}
		}
		return nullptr;
	}

	bool sample_in_list(Sample *sample) {
		for (size_t i = 0; i < num_samples_; ++i) {
			if (sampleList_[i].sample == sample) {
				return true;
			}
		}
		return false;
	}
};

#endif
