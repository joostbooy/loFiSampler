#include "ledPainter.h"

Matrix* LedPainter::matrix_;

LedPainter::Map LedPainter::footer_button_leds_[4] = {
	{ .x = 0, .y = 2 },
	{ .x = 1, .y = 2 },
	{ .x = 2, .y = 2 },
	{ .x = 3, .y = 2 },
};

LedPainter::Map LedPainter::footer_encoder_leds_[4] = {
	{ .x = 0, .y = 2 },
	{ .x = 1, .y = 2 },
	{ .x = 2, .y = 2 },
	{ .x = 3, .y = 2 },
};
LedPainter::Map LedPainter::chapter_leds_[4] = {
	{ .x = 6, .y = 0 },	// Sample
	{ .x = 7, .y = 0 },	// Instrument
	{ .x = 7, .y = 1 },	// Modulation
	{ .x = 0, .y = 4 },	// System
};
