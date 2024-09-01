#ifndef InstrumentSampleListPage_h
#define InstrumentSampleListPage_h

#include "instrument.h"

namespace InstrumentSampleListPage {

	using TopPage::settings_;
	using TopPage::pages_;
	using TopPage::canvas_;

	enum FooterOptions {
		ADD,
		REMOVE,
		CLEAR,
		CLOSE,
		NUM_OPTIONS
	};

	const char* const footer_option_text[NUM_OPTIONS] = { "ADD", "REMOVE", "CLEAR", "CLOSE" };


	void init() {

	}

	void enter() {

	}

	void exit()  {

	}

	void on_encoder(int id, int state) {
		settings_->select_sample_index(settings_->selected_sample_index() + state);
	}

	void on_button(int id, int state) {
		if (state) {
			if (id == Controller::UP_BUTTON) {
				on_encoder(0, -1);
				return;
			}

			if (id == Controller::DOWN_BUTTON) {
				on_encoder(0, 1);
				return;
			}

			switch (Controller::button_to_function(id))
			{
			case ADD:
				if (settings_->selected_instrument().add_sample(settings_->selected_sample())) {
					MessagePainter::show("SAMPLE ADDED");
				}
				break;
			case REMOVE:
				if (settings_->selected_instrument().remove_sample(settings_->selected_instrument_sample_index())) {
					settings_->select_instrument_sample_index(settings_->selected_instrument_sample_index());
					MessagePainter::show("SAMPLE REMOVED");
				}
				break;
			case CLEAR:
				settings_->selected_instrument().clear_samples();
				break;
			case CLOSE:
				pages_->close(Pages::INSTRUMENT_SAMPLE_LIST_PAGE);
				break;
			default:
				break;
			}
		}
	}

	void refresh_leds() {

	}

	// Left : sample list -> Right : instrument sample list
	void draw() {
	//	for (int i = 0; i < settings_->num_samples(); ++i) {
	//		canvas_->draw_text(x, y, settings_->sampleAllocator().read_map(i)->name());
	//		if (i == settings_->selected_sample_index()) {
				// hightlight
	//		}
	//	}
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
