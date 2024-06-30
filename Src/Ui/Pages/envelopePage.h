#ifndef EnvelopePage_h
#define EnvelopePage_h

#include "listPage.h"
#include "envelopeList.h"

namespace EnvelopePage {
	//Declarations
	void init();
	void enter();
	void exit();
	void msTick(uint16_t ticks);
	void drawDisplay();
	void updateLeds();
	void onButton(uint8_t id, uint8_t value);
	void onEncoder(uint8_t id, int inc);
	const uint16_t targetFps();

	// Variables
	bool pasteable_;
	Envelope envelope_;
	EnvelopeList envelopeList;

	void clear() {
		settings.selected_envelope().init();
	}

	void copy() {
		envelope_.paste(&settings.selected_envelope());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
			settings.selected_envelope().paste(&envelope_);
			return true;
		}
		return false;
	}

	void init() {
		pasteable_ = false;
		envelope_.init();
	}

	void enter() {
		ListPage::set_list(&envelopeList);
		ListPage::set_clear_callback(&clear);
		ListPage::set_copy_callback(&copy);
		ListPage::set_paste_callback(&paste);
		ListPage::enter();
	}

	void exit() {
		ListPage::exit();
	}

	void onEncoder(uint8_t id, int inc) {
		ListPage::onEncoder(id, inc);
	}

	void onButton(uint8_t id, uint8_t value) {
		ListPage::onButton(id, value);
	}

	void drawLeds() {
		ListPage::drawLeds();
	}

	void msTick(uint16_t ticks) {
		ListPage::msTick(ticks);
	}

	// Bottom to top
	void drawDisplay() {
		ListPage::drawDisplay();

		Envelope envelope;
		EnvelopeEngine envelopeEngine;

		envelope.paste(&settings.selected_envelope());
		envelope.set_clock_sync(false);
		envelopeEngine.init(&envelope);

		const int x = 128;
		const int y = 5;
		const int w = 32;
		const int h = 32;

		// Todo! figure out how many times we need to loop
		for (int x2 = 0; x2 < 255; ++x2) {
			//int x2 = w * envelopeEngine.phase();
			int y2 = h * (1.f - envelopeEngine.next());
			canvas.draw_pixel(x + x2, y + y2, Canvas::BLACK);
		}

		int index = settings.selected_envelope_index();
		float phase = engine.voiceEngine().most_recent_voice().envelopeEngine(index).phase();

		canvas.vertical_line(x + (phase * w), y, h, Canvas::BLACK);
	}

	const uint16_t targetFps() {
		return ListPage::targetFps();
	}

	const Pages::EventHandlers eventHandlers = {
		&init,
		&enter,
		&exit,
		&msTick,
		&drawLeds,
		&drawDisplay,
		&onEncoder,
		&onButton,
		&targetFps,
	};


};

#endif
