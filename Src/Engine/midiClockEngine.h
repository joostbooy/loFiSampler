#ifndef MidiClock_h
#define MidiClock_h

#include "settings.h"
#include "lookupTables.h"

class MidiClockEngine {

public:

	void init() {
		ext_bpm_ = 120;
		reset();
	}

	void reset() {
		ticks = 0;
		isr_ticks = 0;
		isr_average = 0;
		tempo_phase = 0;
	}

	bool tick() {
		uint16_t bpm;

		if (settings.midi().clock_source() == Midi::INTERNAL) {
			bpm = settings.midi().bpm();
		} else {
			bpm = ext_bpm_;
		}

		++isr_ticks;
		tempo_phase += lut_bpm_inc[bpm];
		return tempo_phase < tempo_inc;
	}

	void sync_bpm() {
		isr_average += isr_ticks;

		if (((++num_readings % 4) == 0) && isr_average > 0) {
			ext_bpm_ = uint32_t(45000000UL * 60UL / 24UL / (kUpdatePeriod * (isr_average / 4)));
			isr_average = 0;
		}

		isr_ticks = 0;
	}

private:
	uint16_t bpm_ = 120;
	uint16_t bpm_fractional_ = 0;
	uint8_t sync_port_;

	volatile uint16_t ext_bpm_;
	volatile uint32_t tempo_inc, tempo_phase, ticks, isr_ticks, isr_average;
	volatile uint8_t num_readings = 0;
	static const uint32_t kUpdatePeriod = 45000000UL / CLOCK_ISR_FREQ;
};

#endif
