#ifndef SamplePage_h
#define SamplePage_h

#include "listPage.h"
#include "sample.h"
#include "sampleList.h"

namespace SamplePage {

	using TopPage::settings_;
	using TopPage::engine_;
	using TopPage::pages_;
	using TopPage::canvas_;

	bool pasteable_;
	Sample sample_;
	SampleList sampleList_;


	enum FooterOptions {
		IMPORT,
		REMOVE,
		NUM_FOOTER_OPTIONS
	};

	const char* const footer_option_text[NUM_FOOTER_OPTIONS] = {
		"IMPORT",
		"REMOVE",
	};

	enum ListOptions {
		CONVERT_TO_MONO,
		MAP_NAME_TO_ROOT_NOTE,
		MAP_NAME_TO_KEY_RANGE,
		CANCEL,

		NUM_LIST_OPTIONS
	};

	const char* const list_option_text[NUM_LIST_OPTIONS] = {
		"CONVERT TO MONO",
		"MAP NAME TO ROOT NOTE",
		"MAP NAME TO KEY RANGE",
		"CANCEL"
	};

	void clear() {
		settings_->selected_sample()->init();
	}

	void copy() {
		sample_.paste(settings_->selected_sample());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
			settings_->selected_sample()->paste(&sample_);
			return true;
		}
		return false;
	}

	void remove(int index) {
		engine_->add_request_blocking(Engine::STOP);

		if (settings_->sampleAllocator().remove(index)) {
			settings_->refresh_sample_selection();
			MessagePainter::show("SAMPLE REMOVED");
		}
	}

	void convert_to_mono(int index) {
		engine_->add_request_blocking(Engine::STOP);

		if (settings_->sampleAllocator().convert_to_mono(index)) {
			MessagePainter::show("SAMPLE CONVERTED TO MONO");
		}
	}

	void map_name_to_root_note(int index) {
		Sample *sample = settings_->sampleAllocator().read_list(index);
		if (sample->map_name_to_root_note()) {
			MessagePainter::show("SUCCES");
		}
	}

	void map_name_to_key_range(int index) {
		Sample *sample = settings_->sampleAllocator().read_list(index);
		if (sample->map_name_to_key_range()) {
			MessagePainter::show("SUCCES");
		}
	}

	void edit(int option) {
		switch (option)
		{
		case CONVERT_TO_MONO:
			ConfirmationPage::set("CONVERT TO MONO ?", [](int option) {
				if (option == ConfirmationPage::CONFIRM) {
					convert_to_mono(settings_->selected_sample_index());
				}
			});
			pages_->open(Pages::CONFIRMATION_PAGE);
			break;
		case MAP_NAME_TO_ROOT_NOTE:
			map_name_to_root_note(settings_->selected_sample_index());
			break;
		case MAP_NAME_TO_KEY_RANGE:
			map_name_to_key_range(settings_->selected_sample_index());
			break;
		case CANCEL:
			break;
		default:
			break;
		}
	}

	void init() {
		pasteable_ = false;
		sample_.init();
		sampleList_.init(engine_, settings_);
	}

	void enter() {
		ListPage::set_list(&sampleList_);
		ListPage::set_clear_callback(&clear);
		ListPage::set_copy_callback(&copy);
		ListPage::set_paste_callback(&paste);
		ListPage::enter();

		OptionListPage::set_text(list_option_text);
		OptionListPage::set_callback(edit);
		OptionListPage::set_count(NUM_LIST_OPTIONS);
	}

	void exit()  {
		ListPage::exit();
	}

	void on_button(int id, int state) {
		ListPage::on_button(id, state);

		if (state) {
			if (id == Controller::MENU_BUTTON) {
				pages_->open(Pages::OPTION_LIST_PAGE);
				return;
			}

			switch (Controller::button_to_function(id))
			{
			case IMPORT:
				pages_->open(Pages::WAV_IMPORT_PAGE);
				break;
			case REMOVE:
				ConfirmationPage::set("REMOVE SAMPLE ?", [](int option) {
					if (option == ConfirmationPage::CONFIRM) {
						remove(settings_->selected_sample_index());
					}
				});
				pages_->open(Pages::CONFIRMATION_PAGE);
				break;
			default:
				break;
			}
		}
	}

	void on_encoder(int id, int state) {
		ListPage::on_encoder(id, state);
	}

	void refresh_leds() {
		ListPage::refresh_leds();
	}

	void draw() {
		ListPage::draw();

		const int x = 2;
		const int y = 5;
		const int w = 251;
		const int h = 32;

		size_t index = 0;
		int16_t *left = 0;
		int16_t *right = 0;

		Sample *sample = settings_->selected_sample();
		size_t inc = sample->size_samples() / w;

		for (int x2 = 0; x2 < w; ++x2) {
			sample->read(index, left, right);

			float left_ = *left * (1.f / 32767.f);
			float right_ = *right * (1.f / 32767.f);
			float mix = (left_ +  right_) * 0.5f;
			int y2 = h * (1.f - mix);

			canvas_->draw_pixel(x + x2, y + y2, Canvas::BLACK);
			index += inc;
		}

		WindowPainter::draw_footer(footer_option_text, NUM_FOOTER_OPTIONS);
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
