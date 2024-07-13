#ifndef ListPage_h
#define ListPage_h

#include "confirmationPage.h"
#include "settingsList.h"

namespace ListPage {

	using TopPage::canvas_;
	using TopPage::pages_;
	using TopPage::leds_;

	SettingsList *list_;
	void(*clear_callback_)() = nullptr;
	bool(*paste_callback_)() = nullptr;
	void(*copy_callback_)() = nullptr;

	void set_list(SettingsList *list) {
		list_ = list;
	}

	void set_clear_callback(void(*callback)()) {
		clear_callback_ = callback;
	}

	void set_paste_callback(bool(*callback)()) {
		paste_callback_ = callback;
	}

	void set_copy_callback(void(*callback)()) {
		copy_callback_ = callback;
	}

	void init() {

	}

	void enter() {

	}

	void exit() {
		clear_callback_ = nullptr;
		paste_callback_ = nullptr;
		copy_callback_ = nullptr;
	}

	void on_button(int id, int state) {
		if (!state) {
			return;
		}

		switch (id)
		{
		case Controller::UP_BUTTON:
			list_->on_up_button();
			break;
		case Controller::DOWN_BUTTON:
			list_->on_down_button();
			break;
		case Controller::CLEAR_BUTTON:
			if (clear_callback_) {
				ConfirmationPage::set("CLEAR SETTINGS ?", [](int option) {
					if (option == ConfirmationPage::CONFIRM) {
						clear_callback_();
					}
				});
				pages_->open(Pages::CONFIRMATION_PAGE);
			}
			break;
		case Controller::COPY_BUTTON:
			if (copy_callback_) {
				copy_callback_();
				MessagePainter::show("SETTINGS COPIED");
			}
			break;
		case Controller::PASTE_BUTTON:
			if (paste_callback_) {
				ConfirmationPage::set("OVERWRITE SETTINGS ?", [](int option) {
					if (option == ConfirmationPage::CONFIRM) {
						if (paste_callback_()) {
							MessagePainter::show("SETTINGS PASTED");
						} else {
							MessagePainter::show("FAILED! CLIPBOARD EMPTY");
						}
					}
				});
				pages_->open(Pages::CONFIRMATION_PAGE);
			}
			break;
		default:
			break;
		}
	}

	void on_encoder(int id, int state) {
		int index = Controller::encoder_to_function(id);
		if (index >= 0) {
			bool pressed = Controller::encoder_is_pressed(id);
			bool shifted = Controller::is_pressed(Controller::SHIFT_BUTTON);
			list_->on_encoder(index, state, pressed || shifted);
		}
	}

	void refresh_leds() {
		for (int i = 0; i < list_->collumns(); ++i) {
			if ((i + list_->top_item()) < list_->num_items()) {
				leds_->set_footer_encoder(i, Leds::RED);
			} else {
				leds_->set_footer_encoder(i, Leds::BLACK);
			}
		}
	}

	void draw_scollbar() {
		const int y = 44;
		const int h = 20;
		const int w = 8;
		const int x = canvas_->width() - w;
		int bar_y = (list_->top_item() * h) / list_->num_items();
		int bar_h = (list_->collumns() * h) / list_->num_items();

		canvas_->fill(x, y, w, h, Canvas::BLACK);
		canvas_->fill(x + 2, y + bar_y, w - 4, bar_h, Canvas::WHITE);
	}

	void draw() {
		const int h = 20;
		const int w = canvas_->width() / list_->collumns();

		canvas_->set_font(Font::SMALL);

		for (int i = 0; i < list_->collumns(); ++i) {
			int item = i + list_->top_item();
			if (item < list_->num_items()) {
				int x = i * w;
				canvas_->draw_text(x, 44, w, h, list_->item_text(item), Canvas::CENTER, Canvas::CENTER);
				canvas_->draw_text(x, 54, w, h, list_->value_text(item), Canvas::CENTER, Canvas::CENTER);
			}
		}

		draw_scollbar();
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
