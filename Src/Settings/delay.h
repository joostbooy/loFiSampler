#ifndef Delay_h
#define Delay_h

class Delay {

public:

	void init() {
		set_amount(0.5f);
		set_feedback(0.5f);
		set_mix(0.0f);
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

	void set_feedback(float value) {
		mix_ = SettingsUtils::clip_float(value);
	}

	const char *mix_text() {
		return SettingsText::float_to_text(mix(), 0, 100);
	}

	// storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(amount_);
		fileWriter.write(feedback_);
		fileWriter.write(mix_);
	}

	void load(FileReader &fileReader) {
		fileReader.read(amount_);
		fileReader.read(feedback_);
		fileReader.read(mix_);
	}

	void paste(Delay *delay) {
		amount_ = delay->amount();
		feedback_ = delay->feedback();
		mix_ = delay->mix();
	}

private:
	float amount_;
	float feedback_;
	float mix_;
};

#endif
