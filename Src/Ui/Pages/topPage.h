#ifndef TopPage_h
#define TopPage_h

#include "settings.h"
#include "engine.h"
#include "canvas.h"
#include "leds.h"
#include "pages.h"
#include "controller.h"
#include "messagePainter.h"
#include "textBufferPainter.h"

namespace TopPage {

	Settings *settings_;
	Engine *engine_;
	Canvas *canvas_;
	Pages *pages_;
	Leds *leds_;
	StringBuilderBase<63>str_;

	void init(Settings *settings, Engine *engine, Canvas *canvas, Leds *leds, Pages *pages) {
		settings_ = settings;
		engine_ = engine;
		canvas_ = canvas;
		leds_ = leds;
		pages_ = pages;
	}

	void enter() {

	}

	void exit() {

	}

	void init() {
		Controller::init();
		MessagePainter::init(canvas_);
		TextBufferPainter::init(canvas_);
	//	WindowPainter::init(canvas_);
	}

	void on_button(int id, int state) {
		Controller::on_button(id, state);
	}

	void on_encoder(int id, int state) {
		Controller::on_encoder(id);
	}

	void refresh_leds() {
		leds_->set_chapter(0);
	}

	void draw() {
		MessagePainter::tick(pages_->target_fps());
		MessagePainter::draw();
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
