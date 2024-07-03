#ifndef ConfirmationPage_h
#define ConfirmationPage_h

#include "page.h"

class ConfirmationPage : public Page {

public:
	typedef void(*Callback)(uint8_t);

	enum Option {
		CONFIRM,
		CANCEL,
		NUM_OPTIONS
	};

	void set(const char* message, Callback callback) {
		message_.write(message);
		callback_ = callback;

	//	w = font.string_width(message_.read()) + 20;
		x = (canvas_->width() - w) / 2;
	}

	void init() override {

	}

	void enter() override {

	}

	void exit() override {

	}

	void on_button(int id, int state) override {
		int selected_option = Controller::button_to_function(id);
		if (state >= 1 && selected_option >= 0 && selected_option < NUM_OPTIONS) {
			callback_(selected_option);
			//pageManager_->close(Pages::CONFIRMATION_PAGE);
		}
	}

	void on_encoder(int id, int state) override {

	}

	void draw_leds() override {
		LedPainter::footer_buttons(Matrix::GREEN, Matrix::RED, Matrix::BLACK, Matrix::BLACK);

	}

	void draw_display() override {
		canvas_->set_font(Font::SMALL);
		canvas_->fill(x + 4, y + 4, w, h, Canvas::SUBTRACTED);
		canvas_->box(x, y, w, h, Canvas::WHITE, Canvas::BLACK);
		canvas_->frame(x + 4, y + 4, w - 8, h - 8, Canvas::WHITE);
		canvas_->draw_text(x, y, w, h, message_.read(), Canvas::CENTER, Canvas::CENTER, Canvas::WHITE);

		//	WindowPainter::draw_footer(option_text, NUM_OPTIONS);
	}

	const size_t target_fps() override {
		return 1000 / 16;
	}

private:
	Callback callback_;
	StringBuilderBase<32>message_;

	uint16_t x = 0;
	uint16_t y = 16;
	uint16_t w = 0;
	uint16_t h = 32;

	const char* const option_text[NUM_OPTIONS] = {
		"CONFIRM",
		"CANCEL"
	};
};

#endif
