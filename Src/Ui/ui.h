#ifndef Ui_h
#define Ui_h

#include "que.h"
#include "canvas.h"
#include "settings.h"
#include "pages.h"
#include "engine.h"
#include "matrix.h"
#include "display.h"
#include <stdint.h>

class Ui {

public:
	enum ControlType {
		BUTTON,
		ENCODER
	};

	struct Event {
		Ui::ControlType type;
		uint8_t id;
		int8_t value;
	};

	void init(Settings*, Engine*, Matrix*, Display*);
	void poll();
	void process();
	void clear_que();
	void send_display();

private:
	Canvas canvas_;
	Pages pages_;

	Engine *engine_;
	Settings *settings_;
	Matrix *matrix_;
	Display *display_;

	uint32_t last_interval = 0;
	uint32_t display_interval = 0;
	uint8_t sw_raw[8 * 8];
};

#endif
