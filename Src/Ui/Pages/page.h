#ifndef Page_h
#define Page_h

#include "pageManager.h"
#include "settings.h"
#include "ledPainter.h"
#include "controller.h"
#include "messagePainter.h"

class Page {

public:

	static void init(Settings *settings, Engine *engine, Canvas *canvas, PageManager *pageManager) {
		settings_ = settings;
		engine_ = engine;
		canvas_ = canvas_;
		pageManager_ = pageManager;
	}

	StringBuilderBase<63>str;

	virtual void init();
	virtual void enter();
	virtual void exit();
	virtual void on_button(int id, int state);
	virtual void on_encoder(int id, int state);
	virtual void draw_leds();
	virtual void draw_display();
	virtual const size_t target_fps();

protected:
	static PageManager *pageManager_;
	static Settings *settings_;
	static Engine *engine_;
	static Canvas *canvas_;
};

#endif
