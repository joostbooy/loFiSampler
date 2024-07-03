#ifndef WindowPainter_h
#define WindowPainter_h

#include "ui.h"
#include "window.h"
#include "settings.h"

class WindowPainter {

public:

	static void init(Ui *ui) {
		ui_ = ui;
	}
	// Window
	static void fill(Window &w, Canvas::Color frame_clr, Canvas::Color fill_clr) {
		ui_->canvas().box(w.x, w.y, w.width, w.height, frame_clr, fill_clr);
	}

	static void shadow(Window &w, int offset) {
		ui_->canvas().fill(w.x - offset, w.y - offset, w.width + (offset * 2), w.height + (offset * 2), Canvas::SUBTRACTED);
	}

	static void text(Window &w, const char* text, Canvas::Allign x, Canvas::Allign y, Canvas::Color color = Canvas::BLACK) {
		ui_->canvas().draw_text(w.x, w.y, w.width, w.height, text, x, y, color);
	}

	static void bitmap(Window &w, Bitmap::Id id, Canvas::Allign x, Canvas::Allign y) {
		ui_->canvas().draw_bitmap(w.x, w.y, w.width, w.height, id, x, y);
	}

	static void vertical_scrollbar(Window &w) {
		if (w.row().scrollable) {
			int x = w.x + w.width - bar_size;
			x = SettingsUtils::clip(w.x, w.x + w.width, x);
			draw_vertical_scrollbar(x, w.y, bar_size, w.height, w.row().items_total, w.row().max_visible, w.row().first);
		}
	}

	static void horizontal_scrollbar(Window &w) {
		if (w.coll().scrollable) {
			int y = w.y + w.height - bar_size;
			y = SettingsUtils::clip(w.y, w.y + w.height, y);
			draw_horizontal_scrollbar(w.x, y, w.width, bar_size, w.coll().items_total, w.coll().max_visible, w.coll().first);
		}
	}

	// Cell
	static void outline(Window::Cell& cell, Canvas::Color frame_clr, Canvas::Color fill_clr) {
		ui_->canvas().box(cell.x, cell.y, cell.w, cell.h, frame_clr, fill_clr);
	}

	static void text(Window::Cell& cell, const char* text, Canvas::Allign x, Canvas::Allign y, Canvas::Color color = Canvas::BLACK) {
		ui_->canvas().draw_text(cell.x + 2, cell.y, cell.w - 2, cell.h, text, x, y, color);
	}

	static void bitmap(Window::Cell& cell, Bitmap::Id id, Canvas::Allign x, Canvas::Allign y = Canvas::CENTER) {
		ui_->canvas().draw_bitmap(cell.x, cell.y, cell.w, cell.h, id, x, y);
	}

	static void highlight(Window::Cell& cell) {
		ui_->canvas().fill(cell.x, cell.y, cell.w, cell.h, Canvas::INVERTED);
	}

	// header & footer
	static void draw_footer(const char* const* text, int num_options) {
		const int num_colls = 4;
		const int h_coll = 8;
		const int w_coll = ui_->canvas().width() / num_colls;
		const int y = ui_->canvas().height() - h_coll;

		ui_->canvas().set_font(Font::SMALL);
		ui_->canvas().fill(0, y, ui_->canvas().width(), h_coll, Canvas::GRAY);

		for (int i = 0; i < num_colls; ++i) {
			int x = i * w_coll;

			ui_->canvas().fill(x, y, w_coll - 1, h_coll, Canvas::LIGHT_GRAY);
			ui_->canvas().horizontal_line(x + 1, y - 1, w_coll - 3, Canvas::GRAY);

			if (i < num_options) {
				ui_->canvas().draw_text(x, y - 1, w_coll, h_coll, text[i], Canvas::CENTER, Canvas::CENTER);
			}
		}
	}

	static void draw_header() {

	}


private:
	static Ui *ui_;

	static const int bar_size = 6;
	static StringBuilderBase<16>str;

	static void draw_vertical_scrollbar(int x, int y, int w, int h, int rows_total, int rows_visible, int curr_top_row) {
		int bar_y = (curr_top_row * h) / rows_total;
		int bar_height = (rows_visible * h) / rows_total;
		ui_->canvas().box(x, y, w, h, Canvas::BLACK, Canvas::WHITE);
		ui_->canvas().box(x + 2, (y + bar_y) + 2, w - 4, SettingsUtils::clip_min(1, bar_height - 4), Canvas::BLACK, Canvas::BLACK);
	}

	static void draw_horizontal_scrollbar(int x, int y, int w, int h, int colls_total, int colls_visible, int curr_left_coll) {
		int bar_x = (curr_left_coll * w) / colls_total;
		int bar_width = (colls_visible * w) / colls_total;
		ui_->canvas().box(x, y, w, h, Canvas::BLACK, Canvas::WHITE);
		ui_->canvas().box((x + bar_x) + 2, y + 2, SettingsUtils::clip_min(1, bar_width - 4), h - 4, Canvas::BLACK, Canvas::BLACK);
	}

};

#endif
