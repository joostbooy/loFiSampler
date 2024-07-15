#ifndef WindowPainter_h
#define WindowPainter_h

#include "canvas.h"

class WindowPainter {

public:

	static void init(Canvas *canvas) {
		canvas_ = canvas;
	}

	static void draw_vertical_scollbar(int x, int y, int w, int h, int top_item, int num_items, int visible_items) {
		int bar_y = (top_item * h) / num_items;
		int bar_h = (visible_items * h) / num_items;

		canvas_->fill(x, y, w, h, Canvas::BLACK);
		canvas_->fill(x + 2, y + bar_y, w - 4, bar_h, Canvas::WHITE);
	}

	static void draw_horizontal_scollbar(int x, int y, int w, int h, int top_item, int num_items, int visible_items) {
		int bar_x = (top_item * w) / num_items;
		int bar_w = (visible_items * w) / num_items;

		canvas_->fill(x, y, w, h, Canvas::BLACK);
		canvas_->fill(bar_x, y + 2, bar_w , h - 4, Canvas::WHITE);
	}

	static void draw_footer(const char* const *text, int num_options) {
		const int h = 10;
		const int y = canvas_->height() - h;
		const int w = canvas_->width() / kMaxFooterOptions;

		if (num_options > kMaxFooterOptions) {
			num_options = kMaxFooterOptions;
		}

		canvas_->horizontal_line(0, y, canvas_->width(), Canvas::BLACK);

		for (int i = 0; i < num_options; ++i) {
			int x = i * w;
			canvas_->vertical_line(x, y, h, Canvas::BLACK);
			canvas_->draw_text(x, y, w, h, text[i], Canvas::CENTER, Canvas::CENTER);
		}
	}

private:
	static Canvas *canvas_;
	static const int kMaxFooterOptions = 4;
};

#endif
