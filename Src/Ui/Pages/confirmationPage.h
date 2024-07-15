#ifndef ConfirmationPage_h
#define ConfirmationPage_h

namespace ConfirmationPage {

	using TopPage::canvas_;
	using TopPage::pages_;
	using TopPage::leds_;

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
		w = canvas_->font().string_width(str.read()) + 20;
		x = (canvas_->width() - w) / 2;
	}

	void init() {

	}

	void enter() {
		if (callback_ == nullptr) {
			pages_->close(Pages::CONFIRMATION_PAGE);
		}
	}

	void exit() {
		callback_ = nullptr;
	}

	void on_button(int id, int state) {
		int selected_option = Controller::button_to_function(id);
		if (state >= 1 && selected_option >= 0 && selected_option < NUM_OPTIONS) {
			callback_(selected_option);
			pages_->close(Pages::CONFIRMATION_PAGE);
		}
	}

	void on_encoder(int id, int state) {

	}

	void refresh_leds() {
		leds_->set_footer_buttons(Leds::RED, Leds::RED, Leds::BLACK, Leds::BLACK);
	}

	void draw() {
		canvas_->set_font(Font::SMALL);
		canvas_->fill(x + 4, y + 4, w, h, Canvas::SUBTRACTED);
		canvas_->box(x, y, w, h, Canvas::WHITE, Canvas::BLACK);
		canvas_->frame(x + 4, y + 4, w - 8, h - 8, Canvas::WHITE);
		canvas_->draw_text(x, y, w, h, str.read(), Canvas::CENTER, Canvas::CENTER, Canvas::WHITE);

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
