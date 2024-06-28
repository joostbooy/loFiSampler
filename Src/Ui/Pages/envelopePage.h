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
	EnvelopeList envelopeList;

	void clear() {
		settings.selected_envelope().init();
	}

	void copy() {
		clipboard.copy(&settings.selected_envelope());
	}

	bool paste() {
		return clipboard.paste(&settings.selected_envelope());
	}

	bool pasteable() {
		return clipboard.envelope_pastable();
	}

	void init() {
		// envelopeList.init();
	}

	void enter() {
		ListPage::set_list(&envelopeList);
		ListPage::set_clear_callback(&clear);
		ListPage::set_copy_callback(&copy);
		ListPage::set_paste_callback(&paste);
		ListPage::set_pasteable_callback(&pasteable);
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
