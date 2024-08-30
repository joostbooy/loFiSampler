#ifndef DiskNavigatorPage_h
#define DiskNavigatorPage_h

#include "topPage.h"
#include "settingsUtils.h"

namespace DiskNavigatorPage {

	using TopPage::canvas_;
	using TopPage::pages_;
	using TopPage::str_;
	using TopPage::leds_;
	using TopPage::settings_;

	Disk *disk_;

	int rowsTotal_;
	int selected_;
	int top_row_;
	void (*callback_)();
	const int kMaxVisibleRows = 4;

	void set_callback(void (*callback)()) {
		callback_ = callback;
	}

	void set_list_filter(Entry::Filter filter) {
		disk_->entry().set_list_filter(filter);
	}

	void refresh_dir() {
		if (!disk_->mounted()) {
			pages_->open(Pages::DISK_RETRY_PAGE);
			return;
		}

		if (!disk_->directory().reopen()) {
			disk_->directory().reset();
		}

		rowsTotal_ = disk_->entry().num_visible();
		disk_->entry().make_list(top_row_, kMaxVisibleRows);
	}

	void scroll_to_row(int row, bool force_refresh = false) {
		int last_top_row = top_row_;

		if (row < top_row_) {
			top_row_ = row;
		} else if (row >= (top_row_ + kMaxVisibleRows)) {
			top_row_ = row - (kMaxVisibleRows - 1);
		}

		if (top_row_ != last_top_row || force_refresh == true) {
			refresh_dir();
		}
	}

	const char* curr_path() {
		return disk_->directory().read_path();
	}

	const char* curr_entry_name() {
		return disk_->entry().read_list(selected_ - top_row_)->name.read();
	}

	const char* curr_entry_path() {
		return str_.write(curr_path(), "/", curr_entry_name());
	}

	bool curr_entry_is_dir() {
		return disk_->entry().read_list(selected_ - top_row_)->is_dir;
	}

	void init() {
		callback_ = nullptr;
		disk_ = settings_->disk();
	}

	void enter() {
		selected_ = 0;
		disk_->directory().reopen();
		scroll_to_row(0, true);
	}

	void exit()  {
		callback_ = nullptr;
		disk_->directory().close();
	}

	void on_encoder(int id, int state) {
		selected_ = SettingsUtils::clip(0, rowsTotal_ - 1, selected_ + state);
		scroll_to_row(selected_);
	}

	void on_button(int id, int state) {
		if (state) {
			Entry::List *e = disk_->entry().read_list(selected_ - top_row_);

			switch (id)
			{
			case Controller::UP_BUTTON:
				on_encoder(0, -1);
				break;
			case Controller::DOWN_BUTTON:
				on_encoder(0, 1);
				break;
			case Controller::LEFT_BUTTON:
				if (disk_->directory().exit()) {
					scroll_to_row(0, true);
				}
				break;
			case Controller::RIGHT_BUTTON:
				if (e->is_dir) {
					if (disk_->directory().enter(e->name.read())) {
						scroll_to_row(0, true);
					}
				}
				break;
			case Controller::MENU_BUTTON:
				if (callback_) {
					callback_();
				}
				break;
			default:
				break;
			}
		}
	}

	void refresh_leds() {
		leds_->set_footer_encoders(4);
		leds_->set_footer_buttons(4);
	}

	void draw() {
		const int row_h = 8;
		const int w = 64;
		const int h = row_h * kMaxVisibleRows;
		const int x = (canvas_->width() - w) / 2;
		const int y = (canvas_->height() - h) / 2;

		canvas_->fill(x, y, w, h, Canvas::WHITE);

		for (int i = 0; i < kMaxVisibleRows; ++i) {
			int row = i + top_row_;
			int row_y = (i * row_h) + y;

			if (row < rowsTotal_) {
				Entry::List* e = disk_->entry().read_list(i);

				if (e->is_dir) {
					canvas_->draw_text(x + 4, row_y, w - 8, row_h, str_.write("/", e->name.read()), Canvas::LEFT, Canvas::CENTER);
				} else {
					canvas_->draw_text(x + 4, row_y, w - 8, row_h, e->name.read(), Canvas::LEFT, Canvas::CENTER);
					canvas_->draw_text(x + 4 + (w / 2), row_y, (w / 2) - 8, row_h, SettingsText::kb_to_mem_size_text(e->size / 1000), Canvas::LEFT, Canvas::CENTER);
				}

				if (row == selected_) {
					canvas_->fill(x, row_y, w, row_h, Canvas::INVERTED);
				}
			}
		}

		const int bar_w = 8;
		const int bar_x = x + (w - bar_w);
		WindowPainter::draw_vertical_scollbar(bar_x, y, bar_w, h, top_row_, rowsTotal_, kMaxVisibleRows);
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
