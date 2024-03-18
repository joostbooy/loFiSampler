#ifndef ModulationEngine_h
#define ModulationEngine_h

#include "dac.h"
#include "lfoEngine.h"
#include "modulation.h"

class ModulationEngine {

public:

	struct Frame {
		float data[Modulation::NUM_DESTINATIONS];
	};

	void init(Modulation *modulation) {
		modulation_ = modulation;
	}

	Frame *frame() {
		return &frame_[0];
	}

	constexpr const size_t frame_size() {
		return kFrameSize;
	}

	void write_mod_env(float value) {
		source_[Modulation::MOD_ENVELOPE] = value;
	}

	void write_cv(int channel, float value) {
		source_[Modulation::CV_1 + channel] = value;
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
		source_[Modulation::LFO_1] = lfoEngine_[0].next();
		source_[Modulation::LFO_2] = lfoEngine_[1].next();
		source_[Modulation::LFO_3] = lfoEngine_[2].next();
		source_[Modulation::LFO_4] = lfoEngine_[3].next();

		for (size_t i = 0; i < kFrameSize; ++i) {
			process(&frame_[i]);
		}
	}

private:
	static const size_t kFrameSize = Dac::kBlockSize;

	Modulation *modulation_;
	Frame frame_[kFrameSize];
	float source_[Modulation::NUM_SOURCES];
	LfoEngine lfoEngine_[settings.num_lfos()];

	void process(Frame *frame) {
		int sources;

		for (int x = 0; x < Modulation::NUM_DESTINATIONS; ++x) {
			sources = 0;
			frame->data[x] = 0.f;
			for (int y = 0; y < Modulation::NUM_SOURCES; ++y) {
				if (modulation_->read_matrix(x, y)) {
					frame->data[x] += source_[y];
					++sources;
				}
			}
			frame->data[x] *= lut_reciprocal[sources];
		}
	}

};

#endif
