#ifndef Delay_h
#define Delay_h

class Delay {

public:

	void init() {
		set_amount(0.5f);
		set_feedback(0.5f);
		set_mix(0.0f);
		set_channel(0);
	}

	// Amount
	float amount() {
		return amount_;
	}

	void set_amount(float value) {
		amount_ = SettingsUtils::clip_float(value);
	}

	const char *amount_text() {
		return SettingsText::float_to_text(amount(), 0, 100);
	}

	// Feedback
	float feedback() {
		return feedback_;
	}

	void set_feedback(float value) {
		feedback_ = SettingsUtils::clip_float(value);
	}

	const char *feedback_text() {
		return SettingsText::float_to_text(feedback(), 0, 100);
	}

	// Mix
	float mix() {
		return mix_;
	}

	void set_mix(float value) {
		mix_ = SettingsUtils::clip_float(value);
	}

	const char *mix_text() {
		return SettingsText::float_to_text(mix(), 0, 100);
	}

	// Channel
	int channel() {
		return channel_;
	}

	void set_channel(int value) {
		channel_ = SettingsUtils::clip(0, (Dac::kNumChannels / 2) - 1, value);
	}

	const char *channel_text() {
		return SettingsText::int_to_text((channel_ * 2) + 1);
	}

	// storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(amount_);
		fileWriter.write(feedback_);
		fileWriter.write(mix_);
		fileWriter.write(channel_);
	}

	void load(FileReader &fileReader) {
		fileReader.read(amount_);
		fileReader.read(feedback_);
		fileReader.read(mix_);
		fileReader.read(channel_);
	}

	void paste(Delay *delay) {
		amount_ = delay->amount();
		feedback_ = delay->feedback();
		mix_ = delay->mix();
		channel_ = delay->channel();
	}

private:
	float amount_;
	float feedback_;
	float mix_;
	int channel_;
};

#endif
