#ifndef LfoPage_h
#define LfoPage_h

#include "listPage.h"
#include "lfoList.h"

namespace LfoPage {
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
	Lfo lfo_;
	LfoList lfoList;

	void clear() {
		settings.selected_lfo().init();
	}

	void copy() {
		lfo_.paste(&settings.selected_lfo());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
			settings.selected_lfo().paste(&lfo_);
			return true;
		}
		return false;
	}

	void init() {
		lfo_.init();
		pasteable_ = false;
	}

	void enter() {
		ListPage::set_list(&lfoList);
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

		Lfo lfo;
		LfoEngine lfoEngine;

		lfo.paste(&settings.selected_lfo());
		lfo.set_randomise(false);
		lfo.set_clock_sync(false);
		lfo.set_speed(1.f);
		lfoEngine.init(&lfo);

		const int x = 128;
		const int y = 5;
		const int w = 32;
		const int h = 32;

		// Todo! figure out how many times we need to loop
		for (int x2 = 0; x2 < 255; ++x2) {
			//int x2 = w * lfoEngine.phase();
			int y2 = h * (1.f - lfoEngine.next());
			canvas.draw_pixel(x + x2, y + y2, Canvas::BLACK);
		}

		int index = settings.selected_lfo_index();
		float phase = engine.modulationEngine().lfoEngine(index).phase();

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
