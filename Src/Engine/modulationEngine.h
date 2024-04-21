#ifndef ModulationEngine_h
#define ModulationEngine_h

#include "dac.h"
#include "lfoEngine.h"
#include "modulation.h"

class ModulationEngine {

public:

	struct Frame {
		float data[Modulation::NUM_DESTINATIONS];

		//	float read(Modulation::Destination dest) {
		//		return data * read_poly(dest);
		//	}
	};

	void init(Settings *settings) {
		modulation_ = &settings->modulation();

		for (size_t i = 0; i < Settings::kNumLfos; ++i) {
			lfoEngine_[i].init(&settings->lfo(i));
		}
	}

	Frame *frame() {
		return &frame_[0];
	}

	constexpr const size_t frame_size() {
		return kFrameSize;
	}

	void write_envelope_1(float value) {
		source_[Modulation::ENVELOPE_1] = value;
	}

	void write_envelope_2(float value) {
		source_[Modulation::ENVELOPE_2] = value;
	}

	void write_cv(int channel, float value) {
		source_[Modulation::CV_1 + channel] = value;
	}

	void write_midi_velocity(float value) {
		source_[Modulation::MIDI_VELOCITY] = value;
	}

	void write_midi_bend(float value) {
		source_[Modulation::MIDI_BEND] = value;
	}

	void write_midi_cc(uint8_t number, float value) {
		for (size_t i = 0; i < Modulation::num_user_cc(); ++i) {
			if (number == modulation_->midi_cc_number(i)) {
				source_[i + Modulation::MIDI_CC_A] = value;
			}
		}
	}

	void fill() {
		for (size_t i = 0; i < kFrameSize; ++i) {
			source_[Modulation::LFO_1] = lfoEngine_[0].next();
			source_[Modulation::LFO_2] = lfoEngine_[1].next();
			source_[Modulation::LFO_3] = lfoEngine_[2].next();
			source_[Modulation::LFO_4] = lfoEngine_[3].next();
			process(&frame_[i]);
		}
	}

private:
	static const size_t kFrameSize = Dac::kBlockSize;

	Modulation *modulation_;
	Frame frame_[kFrameSize];
	float source_[Modulation::NUM_SOURCES];
	LfoEngine lfoEngine_[Settings::kNumLfos];

	void process(Frame *frame) {
		for (int y = 0; y < Modulation::NUM_DESTINATIONS; ++y) {
			frame->data[y] = 1.f;
			for (int x = 0; x < Modulation::NUM_SOURCES; ++x) {
				if (modulation_->read_matrix(x, y) /*&& (source_is_poly(x) == false*/) {
					frame->data[y] *= source_[x];
				}
			}
		}
	}

	//	bool source_is_poly(Modulation::Source src) {
	//		switch (src)
	//		{
//		case Modulation::MIDI_VELOCITY:
//		case Modulation::ENVELOPE_1:
//		case Modulation::ENVELOPE_2:
//			return true;
//		default:
//			break;
//		}
//		return false;
//	}

// float read_poly(Modulation::Destination dest) {
//	float value 1.f;

//	if (modulation_->read_matrix(Modulation::ENVELOPE_1, dest)) {
//		value *= source_[Modulation::ENVELOPE_1];
//	}

//	if (modulation_->read_matrix(Modulation::ENVELOPE_2, dest)) {
//		value *= source_[Modulation::ENVELOPE_2];
//	}

//	if (modulation_->read_matrix(Modulation::MIDI_VELOCITY, dest)) {
//		value *= source_[Modulation::MIDI_VELOCITY];
//	}

//	return value;
//}

};

#endif
