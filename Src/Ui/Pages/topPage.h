#ifndef TopPage_h
#define TopPage_h

#include "pages.h"
#include "canvas.h"
#include "controller.h"
#include "engine.h"
#include "messagePainter.h"
#include "chapterPage.h"

namespace TopPage {
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

	//variables
	StringBuilderBase<64>str;

	void init() {
		Controller::init();
	}

	void enter() {

	}

	void exit() {

	}

	void onEncoder(uint8_t id, int inc) {
		Controller::on_encoder(id);
	}

	void onButton(uint8_t id, uint8_t state) {
		Controller::on_button(id, state);
		ChapterPage::onButton(id, state);
	}

	void drawLeds() {

	}

	void msTick(uint16_t ticks) {

	}

	// Bottom to top
	void drawDisplay() {
		MessagePainter::tick(pages.targetFps());
		MessagePainter::draw();
	}

	const uint16_t targetFps() {
		return 1000 / 16;
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
