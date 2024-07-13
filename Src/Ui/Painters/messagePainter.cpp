#include "messagePainter.h"

uint16_t MessagePainter::x = 0;
uint16_t MessagePainter::y = 16;
uint16_t MessagePainter::w = 0;
uint16_t MessagePainter::h = 32;

Canvas* MessagePainter::canvas_;

bool MessagePainter::is_visible_;
uint16_t MessagePainter::duration_ms_ = 0;
StringBuilderBase<32> MessagePainter::str;
