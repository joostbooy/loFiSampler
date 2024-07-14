#ifndef MessagePainter_h
#define MessagePainter_h

#include "ui.h"
#include "stringBuilder.h"

class MessagePainter {

public:

	static void init(Canvas *canvas) {
		canvas_ = canvas;
	}

	static void show(const char* text, uint16_t millis = 1200) {
		str.write(text);
		w = font.string_width(str.read()) + 20;
		x = (canvas_->width() - w) / 2;
		millis_ = millis;
	}

	static void draw(uint32_t millis) {
		if (millis_ >= millis) {
			millis_ -= millis;
			canvas_->set_font(Font::SMALL);
			canvas_->fill(x + 4, y + 4, w, h, Canvas::SUBTRACTED);
			canvas_->box(x, y, w, h, Canvas::WHITE, Canvas::BLACK);
			canvas_->frame(x + 4, y + 4, w - 8, h - 8, Canvas::WHITE);
			canvas_->draw_text(x, y, w, h, str.read(), Canvas::CENTER, Canvas::CENTER, Canvas::WHITE);
		}
	}

private:
	static int x;
	static int y;
	static int w;
	static int h;
	static Canvas *canvas_;
	static uint16_t millis_;
	static StringBuilderBase<32>str;
};

#endif
