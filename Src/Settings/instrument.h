#ifndef Instrument_h
#define Instrument_h

#include "uiText.h"
#include "sample.h"
#include "fileWriter.h"
#include "fileReader.h"

class Instrument {

public:

	enum SampleRates {
		_2_KHZ,
		_4_KHZ,
		_8_KHZ,
		_10_KHZ,
		_16_KHZ,

		NUM_SAMPLE_RATES
	};

	const char * sample_rate_text(int value) {
		switch (value)
		{
		case _2_KHZ:	return "2 KHZ";
		case _4_KHZ:	return "4 KHZ";
		case _8_KHZ:	return "8 KHZ";
		case _10_KHZ:	return "10 KHZ";
		case _16_KHZ:	return "16 KHZ";
		default:
			break;
		}

		return nullptr;
	}

	void init() {
		set_name("NEW INSTUMENT");
		set_pan(0);
		set_port(0);

		clear_samples();
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
	int8_t pan() {
		return pan_;
	}

	void set_pan(int8_t value) {
		pan_ = stmlib::clip(-100, 100, value);
	}

	const char* pan_text() {
		return UiText::str.write(pan());
	}

	float pan_float() {
		return (1.f / 200.f) * (pan_ + 100);
	}

	// Midi channel
	int8_t midi_channel() {
		return midi_channel_;
	}

	void set_midi_channel(int8_t value) {
		midi_channel_ = stmlib::clip(0, 16, value);
	}

	const char* midi_channel_text() {
		return midi_channel() >= 16 ? "OMNI" : UiText::str.write(midi_channel() + 1);
	}

	bool channel_accepted(int channel) {
		return midi_channel() >= 16 || channel == midi_channel();
	}

	// Bit depth
	int8_t bit_depth() {
		return bit_depth_;
	}

	void set_bit_depth(int8_t value) {
		bit_depth_ = stmlib::clip(1, 12, value);
	}

	const char* bit_depth_text() {
		return UiText::str.write(bit_depth());
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

	const size_t sample_rate_divisor() {
	//	switch (sample_rate())
	//	{
	//	case _2_KHZ: 	return Dac::sampleRate / 2000;
	//	case _4_KHZ: 	return Dac::sampleRate / 4000;
	//	case _8_KHZ:	return Dac::sampleRate / 8000;
	//	case _16_KHZ:	return Dac::sampleRate / 16000;
	//	default:
	//		break;
	//	}
		return 1;
	}

	// Port
	int8_t port() {
		return port_;
	}

	void set_port(int8_t value) {
		port_ = 0; //stmlib::clip(0, Dac::kNumChannels - 1, value);
	}

	const char* port_text() {
		return UiText::str.write(port() + 1);
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
		fileWriter.write(pan_);
	}

	void load(FileReader &fileReader) {
		fileReader.read(pan_);
	}

private:
	int8_t pan_;
	int8_t port_;
	int8_t midi_channel_;
	int8_t bit_depth_;
	int8_t sample_rate_;
	size_t num_samples_;
	static const size_t kMaxNameLength = 16;
	static const size_t kMaxNumSamples = 128;
	char name_[kMaxNameLength];
	Sample *sample_[kMaxNumSamples];

	void swap_samples(int a, int b) {
		Sample *data = sample_[a];
		sample_[a] = sample_[b];
		sample_[b] = data;
	}
};

#endif
