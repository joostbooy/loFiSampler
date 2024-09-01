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
		CONVERT_TO_MONO,

		NUM_OPTIONS
	};

	const char* const footer_option_text[NUM_OPTIONS] = { "IMPORT", "REMOVE", "CONVERT TO MONO" };

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
	}

	void exit()  {
		ListPage::exit();
	}

	void on_button(int id, int state) {
		ListPage::on_button(id, state);

		if (state) {
			switch (Controller::button_to_function(id))
			{
			case IMPORT:
				pages_->open(Pages::WAV_IMPORT_PAGE);
				break;
			case REMOVE:
				remove(settings_->selected_sample_index());
				break;
			case CONVERT_TO_MONO:
				convert_to_mono(settings_->selected_sample_index());
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

		WindowPainter::draw_footer(footer_option_text, NUM_OPTIONS);
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
