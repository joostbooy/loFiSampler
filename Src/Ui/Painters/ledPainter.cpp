#include "ledPainter.h"

Matrix* LedPainter::matrix_;

LedPainter::Map LedPainter::footer_button_leds_[4] = {
	{ .x = 3, .y = 3 },
	{ .x = 4, .y = 3 },
	{ .x = 5, .y = 3 },
	{ .x = 6, .y = 3 },
};

LedPainter::Map LedPainter::footer_encoder_leds_[4] = {
	{ .x = 3, .y = 2 },
	{ .x = 4, .y = 2 },
	{ .x = 5, .y = 2 },
	{ .x = 6, .y = 2 },
};
LedPainter::Map LedPainter::chapter_leds_[6] = {
	{ .x = 1, .y = 0 },	// Sample
	{ .x = 0, .y = 0 },	// Instrument
	{ .x = 1, .y = 1 },	// Modulation
	{ .x = 2, .y = 0 },	// System
	{ .x = 2, .y = 1 },	// Storage
	{ .x = 0, .y = 1 },	// Midi
};
