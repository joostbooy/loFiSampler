#ifndef SamplePage_h
#define SamplePage_h

#include "listPage.h"
#include "sampleList.h"

namespace SamplePage {
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
	Sample sample_;
	SampleList sampleList;

	void clear() {
		settings.selected_sample().init();
	}

	void copy() {
		sample_.paste(&settings.selected_sample());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
			settings.selected_sample().paste(&sample_);
			return true;
		}
		return false;
	}

	void init() {
		pasteable_ = false;
		sample_.init();
	}

	void enter() {
		ListPage::set_list(&sampleList);
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
