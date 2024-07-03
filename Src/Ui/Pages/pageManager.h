#ifndef PageManager_h
#define PageManager_h

#include "settings.h"
#include "engine.h"
#include "canvas.h"

class Page;

class PageManager {

public:

	enum PageId {
		TOP_PAGE,
		LIST_PAGE,
		ENVELOPE_PAGE,
		CONFIRMATION_PAGE,
		NUM_PAGES
	};

	void init(Settings *settings, Engine *engine, Canvas *canvas);
	void open(PageId id);
	void close(PageId id);
	void close_all();
	void on_button(int id, int state);
	void on_encoder(int id, int state);
	void draw_leds();
	void draw_display();
	const size_t target_fps();

	Page* pages(PageId id) {
		return pages_[id];
	}

private:
	static Page *pages_[NUM_PAGES];
	Stack<int, 8>page_stack_;
	int curr_page_;
};

#endif
