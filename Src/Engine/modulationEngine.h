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

	void init(Settings *settings) {
		modulation_ = &settings->modulation();

		for (size_t i = 0; i < Settings::kNumLfos; ++i) {
			lfoEngine_[i].init(&settings->lfo(i));
		}
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

	void retrigger_lfos() {
		for (size_t i = 0; i < Settings::kNumLfos; ++i) {
			lfoEngine_[i].retrigger();
		}
	}

	void tick_lfos() {
		for (size_t i = 0; i < Settings::kNumLfos; ++i) {
			source_[i + Modulation::LFO_1] = lfoEngine_[i].next();
		}
	}

	void write_midi_cc(uint8_t number, float value) {
		for (size_t i = 0; i < Modulation::kNumUserCc; ++i) {
			if (number == modulation_->midi_cc_number(i)) {
				source_[i + Modulation::MIDI_CC_A] = value;
			}
		}
	}

	Frame* process(ModulationMatrix *matrix) {
		for (int y = 0; y < Modulation::NUM_DESTINATIONS; ++y) {
			frame_.data[y] = 1.f;
			for (int x = 0; x < Modulation::NUM_SOURCES; ++x) {
				if (matrix->read(x, y)) {
					frame_.data[y] *= source_[x];
				}
			}
		}

		return &frame_;
	}

private:
	Frame frame_;
	Modulation *modulation_;
	float source_[Modulation::NUM_SOURCES];
	LfoEngine lfoEngine_[Settings::kNumLfos];
};

#endif
