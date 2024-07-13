#ifndef MessagePainter_h
#define MessagePainter_h

#include "ui.h"
#include "stringBuilder.h"

class MessagePainter {

public:

	static void init(Canvas *canvas) {
		canvas_ = canvas;
	}

	static void show(const char* text, uint16_t duration_ms = 1200) {
		str.write(text);
		w = font.string_width(str.read()) + 20;
		x = (canvas_->width() - w) / 2;
		is_visible_ = true;
		duration_ms_ = duration_ms;
	}

	static void draw() {
		if (is_visible_) {
			canvas_->set_font(Font::SMALL);
			canvas_->fill(x + 4, y + 4, w, h, Canvas::SUBTRACTED);
			canvas_->box(x, y, w, h, Canvas::WHITE, Canvas::BLACK);
			canvas_->frame(x + 4, y + 4, w - 8, h - 8, Canvas::WHITE);
			canvas_->draw_text(x, y, w, h, str.read(), Canvas::CENTER, Canvas::CENTER, Canvas::WHITE);
		}
	}

	static void tick(uint16_t ticks) {
		if (duration_ms_ >= ticks){
			duration_ms_ -= ticks;
		} else {
			duration_ms_ = 0;
			is_visible_ = false;
		}
	}

private:
	static Canvas *canvas_;

	static bool is_visible_;
	static uint16_t duration_ms_;
	static StringBuilderBase<32>str;

	static uint16_t x;
	static uint16_t y;
	static uint16_t w;
	static uint16_t h;
};

#endif
