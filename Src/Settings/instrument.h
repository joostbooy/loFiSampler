#ifndef Instrument_h
#define Instrument_h

#include "uiText.h"
#include "dac.h"
#include "fileWriter.h"
#include "fileReader.h"
#include "midiEngine.h"
#include "sample.h"
#include "modulationMatrix.h"

class Instrument {

public:

	enum SampleRates {
		_2_KHZ,
		_4_KHZ,
		_8_KHZ,
		_16_KHZ,

		NUM_SAMPLE_RATES
	};

	const char * sample_rate_text(int value) {
		switch (value)
		{
		case _2_KHZ:	return "2 KHZ";
		case _4_KHZ:	return "4 KHZ";
		case _8_KHZ:	return "8 KHZ";
		case _16_KHZ:	return "16 KHZ";
		default:
			break;
		}

		return nullptr;
	}

	static const size_t kMaxNameLength = 16;
	static const size_t kMaxNumSamples = 128;

	void init(Modulation *modulation) {
		set_name("NEW INSTRUMENT");
		set_gain(1.0);
		set_pan(0.5);
		set_audio_channel(0);
		set_midi_channel(16);
		set_midi_port(Midi::UART);
		set_bit_depth(16);
		set_bend_range(2);
		set_bend(0.5);

		matrix().init(modulation);

		clear_samples();
	}

	// matrix
	ModulationMatrix& matrix() {
		return matrix_;
	}

	// Sample
	Sample *sample(uint8_t index) {
		return sample_[index];
	}

	size_t num_samples() {
		return num_samples_;
	}

	void clear_samples() {
		num_samples_ = 0;
	}

	bool add_sample(Sample *sample) {
		if (num_samples_ < kMaxNumSamples) {
			sample_[num_samples_] = sample;
			++num_samples_;
			return true;
		}
		return false;
	}

	bool remove_sample(uint8_t index) {
		if (index >= num_samples()) {
			return false;
		}

		for (int i = 0; i < index; ++i) {
			sample_[i] = sample_[i + 1];
		}
		--num_samples_;
		return true;
	}

	bool swap_sample_with_left(uint8_t index) {
		int left = index - 1;
		if (left >= 0 && index < num_samples()) {
			swap_samples(left, index);
			return true;
		}

		return false;
	}

	bool swap_sample_with_right(uint8_t index) {
		int right = index + 1;
		int far_right = num_samples() - 1;

		if (far_right > 0 && index < far_right) {
			swap_samples(index, right);
			return true;
		}

		return false;
	}

	// pan
	float pan() {
		return pan_;
	}

	void set_pan(float value) {
		pan_ = stmlib::clip_float(value);
	}

	const char* pan_text() {
		return UiText::float_to_text(-100, 100, pan());
	}

	// Midi channel
	int8_t midi_channel() {
		return midi_channel_;
	}

	void set_midi_channel(int8_t value) {
		midi_channel_ = stmlib::clip(0, 16, value);
	}

	const char* midi_channel_text() {
		return UiText::midi_channel_text(midi_channel());
	}

	bool midi_channel_accepted(int channel) {
		return midi_channel() >= 16 || channel == midi_channel();
	}

	// Midi port
	int8_t midi_port() {
		return midi_port_;
	}

	void set_midi_port(int8_t value) {
		midi_port_ = stmlib::clip(0, Midi::NUM_PORTS - 1, value);
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
		bit_depth_ = stmlib::clip(2, 16, value);
	}

	uint8_t bit_depth() {
		return bit_depth_;
	}

	const char *bit_depth_text() {
		return UiText::str.write(bit_depth(), " BIT");
	}

	uint8_t bit_shifts() {
		return 16 - bit_depth();
	}

	// SampleRate
	int8_t sample_rate() {
		return sample_rate_;
	}

	void set_sample_rate_(int8_t value) {
		sample_rate_ = stmlib::clip(0, NUM_SAMPLE_RATES - 1, value);
	}

	const char* sample_rate_text() {
		return sample_rate_text(sample_rate());
	}

	const size_t sample_rate_prescaler() {
		switch (sample_rate())
		{
		case _2_KHZ: 	return Dac::kSampleRate / 2000;
		case _4_KHZ: 	return Dac::kSampleRate / 4000;
		case _8_KHZ:	return Dac::kSampleRate / 8000;
		case _16_KHZ:	return Dac::kSampleRate / 16000;
		default:
			break;
		}
		return 1;
	}

	// Audio channel
	int8_t audio_channel() {
		return audio_channel_;
	}

	void set_audio_channel(int8_t value) {
		audio_channel_ = stmlib::clip(0, (Dac::kNumChannels / 2) - 1, value);
	}

	const char* audio_channel_text() {
		int chn = (audio_channel() * 2) + 1;
		return UiText::str.write(chn, " & ", chn + 1);
	}

	// Bend range
	uint8_t bend_range() {
		return bend_range_;
	}

	void set_bend_range(int value) {
		bend_range_ = stmlib::clip(0, 24, value);
	}

	const char* bend_range_text() {
		return UiText::str.write(bend_range(), " SEMITONES");
	}

	// Bend
	float bend() {
		return bend_;
	}

	void set_bend(float value) {
		bend_ = stmlib::clip_float(value);
	}

	const char* bend_text() {
		return UiText::str.write(Dsp::cross_fade(-1, 1, bend()));
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
		gain_ = stmlib::clip_float(value);
	}

	const char *gain_text() {
		return UiText::percentage_to_text(gain() * 100, 100);
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
		fileWriter.write(sample_rate_);
		fileWriter.write(bend_range_);
		fileWriter.write(num_samples_);

		matrix().save(fileWriter);
	}

	void load(FileReader &fileReader) {
		fileReader.read(pan_);
		fileReader.read(gain_);
		fileReader.read(bend_);
		fileReader.read(audio_channel_);
		fileReader.read(midi_port_);
		fileReader.read(midi_channel_);
		fileReader.read(bit_depth_);
		fileReader.read(sample_rate_);
		fileReader.read(bend_range_);
		fileReader.read(num_samples_);

		matrix().load(fileReader);
	}

	void paste(Instrument *instrument) {
		pan_ = instrument->pan();
		gain_ = instrument->gain();
		bend_ = instrument->bend();
		audio_channel_ = instrument->audio_channel();
		midi_port_ = instrument->midi_port();
		midi_channel_ = instrument->midi_channel();
		bit_depth_ = instrument->bit_depth();
		sample_rate_ = instrument->sample_rate();
		bend_range_ = instrument->bend_range();
		bit_depth_ = instrument->bit_depth();
		num_samples_ = instrument->num_samples();
	}

private:
	float pan_;
	float gain_;
	float bend_;
	int8_t audio_channel_;
	int8_t midi_port_;
	int8_t midi_channel_;
	int8_t bit_depth_;
	int8_t sample_rate_;
	uint8_t bend_range_;
	size_t num_samples_;

	char name_[kMaxNameLength];
	Sample *sample_[kMaxNumSamples];

	ModulationMatrix matrix_;

	void swap_samples(int a, int b) {
		Sample *data = sample_[a];
		sample_[a] = sample_[b];
		sample_[b] = data;
	}
};

#endif
