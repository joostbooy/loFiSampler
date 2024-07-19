#ifndef Delay_h
#define Delay_h

class Delay {

public:

	static const size_t kMaxDelay = SAMPLE_RATE * 2;

	void init() {
		set_amount(0.5f);
		set_feedback(0.5f);
		set_mix(0.0f);
		set_channel(0);
		set_sync(false);
	}

	// Amount
	float amount() {
		return amount_;
	}

	void set_amount(float value) {
		amount_ = SettingsUtils::clip_float(value);
	}

	const char *amount_text() {
		if (sync()) {
			return nullptr;
		} else {
			return SettingsText::samples_to_time(amount() * kMaxDelay);
		}
	}

	size_t num_samples() {
		if (sync()) {
			return 0; //SAMPLE_RATE * MidiSync::read_inc(amount() * MidiSync::max_value);
		} else {
			return amount() * kMaxDelay;
		}
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
		channel_ = SettingsUtils::clip(0, Dac::kNumStereoChannels - 1, value);
	}

	const char *channel_text() {
		return SettingsText::audio_channel_to_text(channel());
	}

	// Sync
	bool sync() {
		return sync_;
	}

	void set_sync(bool value) {
		sync_ = value;
	}

	const char *sync_text() {
		return SettingsText::bool_to_on_off(sync());
	}

	// storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(amount_);
		fileWriter.write(feedback_);
		fileWriter.write(mix_);
		fileWriter.write(channel_);
		fileWriter.write(sync_);
	}

	void load(FileReader &fileReader) {
		fileReader.read(amount_);
		fileReader.read(feedback_);
		fileReader.read(mix_);
		fileReader.read(channel_);
		fileReader.read(sync_);
	}

	void paste(Delay *delay) {
		mix_ = delay->mix();
		amount_ = delay->amount();
		feedback_ = delay->feedback();
		channel_ = delay->channel();
		sync_ = delay->sync();
	}

private:
	float amount_;
	float feedback_;
	float mix_;
	int channel_;
	bool sync_;
};

#endif
