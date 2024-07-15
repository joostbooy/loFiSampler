#ifndef OptionListPage_h
#define OptionListPage_h

#include "topPage.h"
#include "settingsUtils.h"

namespace OptionListPage {

	using TopPage::canvas_;
	using TopPage::pages_;
	using TopPage::leds_;

	int count_;
	int selected_;
	int top_row_;
	const char* const* text_;
	void (*callback_)(int);
	const int kMaxRows = 4;

	void set_text(const char* const* text) {
		text_ = text;
	}

	void set_callback(void (*callback)(int)) {
		callback_ = callback;
	}

	void set_count(int count) {
		count_ = count;
	}

	void scroll_to_row(int row) {
		if (row < top_row_) {
			top_row_ = row;
		} else if (row >= (top_row_ + kMaxRows)) {
			top_row_ = row - (kMaxRows - 1);
		}
	}

	void init() {
		callback_ = nullptr;
	}

	void enter() {
		selected_ = 0;
		scroll_to_row(0);
	}

	void exit()  {
		callback_ = nullptr;
	}

	void on_button(int id, int state) {
		if (state) {
			if (Controller::button_to_function(id) >= 0) {
				if (callback_) {
					callback_(selected_);
				}
				pages_->close(Pages::OPTION_LIST_PAGE);
			}
		}
	}

	void on_encoder(int id, int state) {
		selected_ = SettingsUtils::clip(0, count_ - 1, selected_ + state);
		scroll_to_row(selected_);
	}

	void refresh_leds() {
		leds_->set_footer_encoders(4);
		leds_->set_footer_buttons(4);
	}

	void draw() {
		const int row_h = 8;
		const int w = 64;
		const int h = row_h * kMaxRows;
		const int x = (canvas_->width() - w) / 2;
		const int y = (canvas_->height() - h) / 2;

		canvas_->fill(x, y, w, h, Canvas::WHITE);

		for (int i = 0; i < kMaxRows; ++i) {
			int row = i + top_row_;
			if (row < count_) {
				canvas_->draw_text(x + 4, y, w - 8, row_h, text_[row], Canvas::LEFT, Canvas::CENTER);
				if (row == selected_) {
					canvas_->fill(x, y, w, row_h, Canvas::INVERTED);
				}
			}
		}

 		const int bar_w = 8;
		const int bar_x = x + (w - bar_w);
		WindowPainter::draw_vertical_scollbar(bar_x, y, bar_w, h, top_row_, count_, kMaxRows);
	}

	const size_t target_fps() {
		return 1000 / 16;
	}

	Pages::Page page = {
		&init,
		&enter,
		&exit,
		&draw,
		&refresh_leds,
		&on_button,
		&on_encoder,
		&target_fps
	};

};

#endif
