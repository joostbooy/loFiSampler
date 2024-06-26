#ifndef lookupTables_h
#define lookupTables_h

#include <stdint.h>

#define SAMPLE_RATE 16000
#define PPQN 24
#define MAX_BPM 300
#define CLOCK_ISR_FREQ 8000
#define EXP_TABLE_SIZE 1024
#define PHASE_TABLE_SIZE 256

extern const uint32_t lut_bpm_inc[];
extern const float lut_phase_inc[];
extern const float lut_tempo_sync_phase_inc[];
extern const float lut_reciprocal[];
extern const float lut_exp[];
extern const float lut_inv_exp[];
extern const float lut_semitone_ratio[];
extern const float lut_cent_ratio[];

#endif