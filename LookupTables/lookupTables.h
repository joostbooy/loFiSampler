#ifndef lookupTables_h
#define lookupTables_h

#include <stdint.h>

#define BLOCK_SIZE 8
#define SAMPLE_RATE 16000
#define MIN_BPM 30
#define MAX_BPM 300
#define CLOCK_ISR_FREQ 4000
#define EXP_TABLE_SIZE 1024
#define PHASE_TABLE_SIZE 256

extern const uint32_t lut_bpm_inc[];
extern const float lut_phase_inc[];
extern const float lut_sync_phase_inc[];
extern const float lut_beat_length[];
extern const float lut_exp[];
extern const float lut_inv_exp[];
extern const float lut_semitone_ratio[];
extern const float lut_cent_ratio[];

#endif