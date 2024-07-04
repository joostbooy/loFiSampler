#ifndef Pages_h
#define Pages_h

#include "settings.h"
#include "engine.h"
#include "canvas.h"

class Pages {

public:

	enum PageId {
		TOP_PAGE,
		LIST_PAGE,
		LFO_PAGE,
		ENVELOPE_PAGE,
		INSTRUMENT_PAGE,
		CONFIRMATION_PAGE,
		NUM_PAGES
	};

	void init(Settings *settings, Engine *engine, Canvas *canvas);
	void open(PageId id);
	void close(PageId id);
	void close_all();
	void on_button(int id, int state);
	void on_encoder(int id, int state);
	void draw();
	void refresh_leds();
	const size_t target_fps();

	struct Page {
		void(*init)();
		void(*enter)();
		void(*exit)();
		void(*draw)();
		void(*refresh_leds)();
		void(*on_button)(int id, int state);
		void(*on_encoder)(int id, int state);
		const size_t (*target_fps)();
	};

private:
	int curr_page_;
	Stack<int, 8>page_stack_;
	Page *page_[NUM_PAGES];
};

#endif
