#ifndef EnvelopePage_h
#define EnvelopePage_h

#include "listPage.h"
#include "envelope.h"
#include "envelopeList.h"

namespace EnvelopePage {

	bool pasteable_;
	Envelope envelope_;
	EnvelopeList envelopeList_;

	void clear() {
		//settings_->selected_envelope().init();
	}

	void copy() {
		//envelope_.paste(&settings_->selected_envelope());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
		//	settings_->selected_envelope().paste(&envelope_);
			return true;
		}
		return false;
	}

	void init() {
		pasteable_ = false;
		envelope_.init();
	//	envelopeList_.init(engine_, settings_);
	}

	void enter() {
		ListPage::set_list(&envelopeList_);
		ListPage::set_clear_callback(&clear);
		ListPage::set_copy_callback(&copy);
		ListPage::set_paste_callback(&paste);
	}

	void exit()  {

	}

	void on_button(int id, int state) {
		ListPage::on_button(id, state);
	}

	void on_encoder(int id, int state) {
		ListPage::on_button(id, state);
	}

	void refresh_leds() {
		ListPage::refresh_leds();
	}

	void draw() {
		ListPage::draw();

		Envelope envelope;
		EnvelopeEngine envelopeEngine;

//		envelope.paste(&settings_->selected_envelope());
		envelope.set_clock_sync(false);
		envelopeEngine.init(&envelope);

		const int x = 128;
		const int y = 5;
		const int w = 32;
		const int h = 32;

		for (int x2 = 0; x2 < 255; ++x2) {
			int y2 = h * (1.f - envelopeEngine.next());
		//	canvas_->draw_pixel(x + x2, y + y2, Canvas::BLACK);
		}

		// int index = settings_->selected_envelope_index();
		// float phase = engine_->voiceEngine().most_recent_voice().envelopeEngine(index).phase();

	//	canvas_->vertical_line(x + (phase * w), y, h, Canvas::BLACK);
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
