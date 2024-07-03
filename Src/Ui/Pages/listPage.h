#ifndef ListPage_h
#define ListPage_h

#include "page.h"
#include "settingsList.h"

class ListPage : public Page {

public:

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


	void init() override {

	}

	void enter() override {

	}

	void exit() override {
		clear_callback_ = nullptr;
		paste_callback_ = nullptr;
		copy_callback_ = nullptr;
	}

	void on_button(int id, int state) override {
		/*
		if (id == Controller::UP_BUTTON && value > 0) {
			list_->on_up_button();
			return;
		}

		if (id == Controller::DOWN_BUTTON && value > 0) {
			list_->on_down_button();
			return;
		}

		if (id == Controller::CLEAR_BUTTON && value >= 1 && clear_callback_ != nullptr) {
			ConfirmationPage::set("CLEAR ALL SETTINGS ?", [](uint8_t option) {
				if (option == ConfirmationPage::CONFIRM) {
					clear_callback_();
				}
			});
			TopPage::ui_->pages().open(Pages::CONFIRMATION_PAGE);
			return;
		}

		if (id == Controller::COPY_BUTTON && value >= 1 && copy_callback_ != nullptr) {
			copy_callback_();
			MessagePainter::show("SETTINGS COPIED");
			return;
		}

		if (id == Controller::PASTE_BUTTON && value >= 1 && paste_callback_ != nullptr) {
			ConfirmationPage::set("OVERWRITE SETTINGS ?", [](uint8_t option) {
				if (option == ConfirmationPage::CONFIRM) {
					if (paste_callback_()) {
						MessagePainter::show("SETTINGS PASTED");
					} else {
						MessagePainter::show("FAILED! CLIPBOARD EMPTY");
					}
				}
			});
			TopPage::ui_->pages().open(Pages::CONFIRMATION_PAGE);
			return;
		}
*/
	}

	void on_encoder(int id, int state) override {
		/*
		int index = Controller::encoder_to_function(id);
		if (index >= 0) {
			bool pressed = Controller::encoder_is_pressed(index);
			bool shifted = Controller::is_pressed(Controller::SHIFT_BUTTON);
			list_->on_encoder(index, inc, pressed || shifted);
		}
		*/
	}

	void draw_leds() override {
		if (clear_callback_) {
			LedPainter::set_clear(Matrix::GREEN);
		}

		LedPainter::set_y(Matrix::GREEN);
		LedPainter::set_edit(Matrix::GREEN);
		LedPainter::set_menu(Matrix::GREEN);

		if (copy_callback_ && paste_callback_) {
			LedPainter::set_copy(Matrix::GREEN);
		}
	}

	void draw_display() override {
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

	const size_t target_fps() override {
		return 1000 / 16;
	}

private:
	void(*clear_callback_)() = nullptr;
	void(*copy_callback_)() = nullptr;
	bool(*paste_callback_)() = nullptr;

	SettingsList *list_;
};

#endif
