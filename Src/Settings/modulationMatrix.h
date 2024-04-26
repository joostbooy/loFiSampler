#ifndef ModulationMatrix_h
#define ModulationMatrix_h

#include "modulation.h"

class ModulationMatrix {

public:

	void init() {
		clear();
		toggle(Modulation::ENVELOPE_1, Modulation::SAMPLE_GAIN);
		toggle(Modulation::MIDI_VELOCITY, Modulation::SAMPLE_GAIN);
	}

	bool read(size_t src, size_t dest) {
		return matrix_[src] & (1 << dest);
	}

	void clear() {
		for (size_t i = 0; i < Modulation::NUM_SOURCES; ++i) {
			matrix_[i] = 0;
		}
	}

	void toggle(size_t src, size_t dest) {
		uint32_t data = matrix_[src];

		if (read(src, dest)) {
			matrix_[src] = data & ~(1 << dest);
		} else {
			matrix_[src] = data | (1 << dest);
		}
	}


	// Storage
	void save(FileWriter &fileWriter) {
		for (size_t i = 0; i < Modulation::NUM_SOURCES; ++i) {
			fileWriter.write(matrix_[i]);
		}
	}

	void load(FileReader &fileReader) {
		for (size_t i = 0; i < Modulation::NUM_SOURCES; ++i) {
			fileReader.read(matrix_[i]);
		}
	}

	void paste(ModulationMatrix *modulationMatrix) {
		clear();
		for (size_t y = 0; y < Modulation::NUM_SOURCES; ++y) {
			for (size_t x = 0; x < Modulation::NUM_DESTINATIONS; ++x) {
				if (modulationMatrix->read(x, y)) {
					toggle(x, y);
				}
			}
		}
	}

private:
	uint32_t matrix_[Modulation::NUM_SOURCES];
};

#endif
