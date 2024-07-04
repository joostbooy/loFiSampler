#ifndef ConfirmationPage_h
#define ConfirmationPage_h

namespace ConfirmationPage {

	enum Option {
		CONFIRM,
		CANCEL,
		NUM_OPTIONS
	};

	const char* const option_text[NUM_OPTIONS] = {
		"CONFIRM",
		"CANCEL"
	};


	typedef void(*Callback)(int);
	Callback callback_;
	StringBuilderBase<32>str;

	int x = 0;
	int y = 16;
	int w = 0;
	int h = 32;

	void set(const char* message, Callback callback) {
		str.write(message);
		callback_ = callback;
		w = font.string_width(str.read()) + 20;
		x = (TopPage::canvas_->width() - w) / 2;
	}

	void init() {

	}

	void enter() {

	}

	void exit() {

	}

	void on_button(int id, int state) {
		int selected_option = Controller::button_to_function(id);
		if (state >= 1 && selected_option >= 0 && selected_option < NUM_OPTIONS) {
			callback_(selected_option);
			TopPage::pages_->close(Pages::CONFIRMATION_PAGE);
		}
	}

	void on_encoder(int id, int state) {

	}

	void refresh_leds() {
		LedPainter::footer_buttons(Matrix::GREEN, Matrix::RED, Matrix::BLACK, Matrix::BLACK);

	}

	void draw() {
		TopPage::canvas_->set_font(Font::SMALL);
		TopPage::canvas_->fill(x + 4, y + 4, w, h, Canvas::SUBTRACTED);
		TopPage::canvas_->box(x, y, w, h, Canvas::WHITE, Canvas::BLACK);
		TopPage::canvas_->frame(x + 4, y + 4, w - 8, h - 8, Canvas::WHITE);
		TopPage::canvas_->draw_text(x, y, w, h, str.read(), Canvas::CENTER, Canvas::CENTER, Canvas::WHITE);

		//	WindowPainter::draw_footer(option_text, NUM_OPTIONS);
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
