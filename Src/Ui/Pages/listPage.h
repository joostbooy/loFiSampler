#ifndef ListPage_h
#define ListPage_h

#include "confirmationPage.h"
#include "settingsList.h"

namespace ListPage {

	SettingsList *list_;
	void(*clear_callback_)() = nullptr;
	bool(*paste_callback_)() = nullptr;
	void(*copy_callback_)() = nullptr;

	void set_list(SettingsList *list) {
		list_ = list;
		list_->set_top_item(0);
		//	window.set_row_items_total(list_->num_items());
		//	window.scroll_to_row(list_->top_item());
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
				TopPage::pages_->open(Pages::CONFIRMATION_PAGE);
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
				TopPage::pages_->open(Pages::CONFIRMATION_PAGE);
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
		LedPainter::set_shift(Matrix::RED);

		if (clear_callback_) {
			LedPainter::set_clear(Matrix::RED);
		}

		if (copy_callback_ && paste_callback_) {
			LedPainter::set_copy(Matrix::RED);
			LedPainter::set_paste(Matrix::RED);
		}
	}

	void draw() {
		/*
		WindowPainter::draw_header();

		Canvas::Color color;
		TopPage::canvas_->set_font(Font::SMALL);
		int item = list_->top_item();

		for (int i = window.row().first; i <= window.row().last; ++i) {
		color = (item == i) ? Canvas::BLACK : Canvas::LIGHT_GRAY;
		WindowPainter::text(window.cell(0, i), list_->item_text(i), Canvas::LEFT, Canvas::CENTER, color);
		WindowPainter::text(window.cell(1, i), list_->value_text(i), Canvas::LEFT, Canvas::CENTER, color);
	}

	WindowPainter::vertical_scrollbar(window);
	*/
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
