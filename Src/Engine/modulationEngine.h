#ifndef ModulationEngine_h
#define ModulationEngine_h

#include "dac.h"

class ModulationEngine {

public:

	struct Frame {
		float pan;
		float bend;
		float gain;
	};

	void init() {

	}

	Frame *frame() {
		return &frame_[0];
	}

	constexpr const size_t frame_size() {
		return Dac::kBlockSize;
	}

	void fill() {
		for (int i = 0; i < frame_size(); ++i) {
			frame_[i].bend = 0.f;
			frame_[i].gain = 0.f;
		}
	}

private:
	Frame frame_[frame_size()];
	//LfoEngine lfoEngine_[settings.num_lfos()];
	//EnvelopeEngine envelopeEngine_[kMaxVoices];
};

#endif
