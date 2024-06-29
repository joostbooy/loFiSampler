#ifndef InstrumentPage_h
#define InstrumentPage_h

#include "listPage.h"
#include "instrumentList.h"

namespace InstrumentPage {
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
	enum FooterOptions {
		MOD_MATRIX,
		SAMPLES,

		NUM_FOOTER_OPTIONS
	};

	const char* const footer_option_text[NUM_FOOTER_OPTIONS] = { "MOD MATRIX", "SAMPLES" };

	bool pasteable_;
	Instrument instument_;
	InstrumentList instrumentList;

	void clear() {
		settings.selected_instrument().init();
	}

	void copy() {
		instument_.paste(&settings.selected_instrument());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
			settings.selected_instrument().paste(&instument_);
			return true;
		}
		return false;
	}

	void init() {
		pasteable_ = false;
		instument_.init();
	}

	void enter() {
		ListPage::set_list(&instrumentList);
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

		if (value > 0) {
			switch (controller.button_to_function(id))
			{
			case MOD_MATRIX:
				//pages.open(Pages::MOD_MATRIX_PAGE);
				break;
			default:
				break;
			}
		}
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
		//FooterPage::drawDisplay();
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
