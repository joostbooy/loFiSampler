#ifndef ModulationMatrixPage_h
#define ModulationMatrixPage_h

#include "topPage.h"

namespace ModulationMatrixPage {

	using TopPage::settings_;
	using TopPage::canvas_;
	using TopPage::leds_;
	using TopPage::pages_;

	int src_ = 0;
	int top_src_ = 0;
	const int kMaxVisibleSources = 4;

	int dest_ = 0;
	int top_dest_ = 0;
	const int kMaxVisibleDestinations = 4;

	bool pasteable_ = false;
	ModulationMatrix modulationMatrix_;

	enum FooterOptions {
		TOGGLE,
		CLEAR,
		NUM_FOOTER_OPTIONS
	};

	const char *const footer_text[NUM_FOOTER_OPTIONS] = { "TOGGLE", "CLEAR" };

	void scroll_to_source(int src) {
		src_ = SettingsUtils::clip(0, ModulationMatrix::NUM_SOURCES - 1, src);

		if (src_ < top_src_) {
			top_src_ = src_;
		} else if (src_ >= (top_src_ + kMaxVisibleSources)) {
			top_src_ = src_ - (kMaxVisibleSources - 1);
		}
	}

	void scroll_to_destination(int dest) {
		dest_ = SettingsUtils::clip(0, ModulationMatrix::NUM_DESTINATIONS - 1, dest);

		if (dest_ < top_dest_) {
			top_dest_ = dest_;
		} else if (dest_ >= (top_dest_ + kMaxVisibleDestinations)) {
			top_dest_ = dest_ - (kMaxVisibleDestinations - 1);
		}
	}

	void init() {

	}

	void enter() {

	}

	void exit()  {

	}

	void on_encoder(int id, int state) {
		if (id == Controller::FUNCTION_ENC_A) {
			scroll_to_source(src_ + state);
		} else if (id == Controller::FUNCTION_ENC_D) {
			scroll_to_destination(dest_ + state);
		}
	}

	void on_button(int id, int state) {
		if (state) {

			switch (id)
			{
			case Controller::UP_BUTTON:
				on_encoder(Controller::FUNCTION_ENC_A, -1);
				break;
			case Controller::DOWN_BUTTON:
				on_encoder(Controller::FUNCTION_ENC_A, 1);
				break;
			case Controller::LEFT_BUTTON:
				on_encoder(Controller::FUNCTION_ENC_D, -1);
				break;
			case Controller::RIGHT_BUTTON:
				on_encoder(Controller::FUNCTION_ENC_D, 1);
				break;
			case Controller::COPY_BUTTON:
				modulationMatrix_.paste(&settings_->selected_modulation_matrix());
				pasteable_ = true;
				break;
			case Controller::PASTE_BUTTON:
				if (pasteable_) {
					ConfirmationPage::set("OVERWRITE MATRIX ?", [](int option) {
						if (option == ConfirmationPage::CONFIRM) {
							settings_->selected_modulation_matrix().paste(&modulationMatrix_);
						}
					});
					pages_->open(Pages::CONFIRMATION_PAGE);
				}
				break;
			default:
				break;
			}

			switch (Controller::button_to_function(id))
			{
			case TOGGLE:
				settings_->selected_modulation_matrix().toggle(src_, dest_);
				break;
			case CLEAR:
				ConfirmationPage::set("CLEAR MATRIX ?", [](int option) {
					if (option == ConfirmationPage::CONFIRM) {
						settings_->selected_modulation_matrix().clear();
					}
				});
				pages_->open(Pages::CONFIRMATION_PAGE);
				break;
			default:
				break;
			}
		}
	}

	void refresh_leds() {
		leds_->set_footer_encoder(0, Leds::RED);
		leds_->set_footer_encoder(1, Leds::BLACK);
		leds_->set_footer_encoder(2, Leds::BLACK);
		leds_->set_footer_encoder(3, Leds::RED);

		leds_->set_footer_buttons(NUM_FOOTER_OPTIONS);
	}

	void draw() {
		//	const int coll_w = 0;
		//	const int row_h = 0;
		//	const int x = 0;
		//	const int y = 0;
		//	const int w = 0;
		//	const int h = 0;

		for (int src = 0; src < kMaxVisibleSources; ++src) {
			for (int dest = 0; dest < kMaxVisibleDestinations; ++dest) {
				//	bool state = settings_->selected_modulation_matrix().read(src, dest);
				if (src == src_ && dest == dest_ ) {
					// hightlight
				}
			}
		}
		//WindowPainter::draw_vertical_scollbar();
		//WindowPainter::draw_horizontal_scollbar();

		WindowPainter::draw_footer(footer_text, NUM_FOOTER_OPTIONS);
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
