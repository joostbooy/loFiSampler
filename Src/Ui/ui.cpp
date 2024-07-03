#include "ui.h"
#include "que.h"
#include "controller.h"

Que<Ui::Event, 16> ui_que;

static const int num_enc_rows = 2;
static const int num_buttons_rows = 4;
static const int enc_a_rows[num_enc_rows] = { 1, 4 };
static const int enc_b_rows[num_enc_rows] = { 2, 5 };
static const int buttons_rows[num_buttons_rows] = { 0, 3, 6, 7 };

void addEvent(Ui::ControlType type, uint8_t id, int8_t value)  {
	Ui::Event e;
	e.type = type;
	e.id = id;
	e.value = value;
	ui_que.write(e);
}

void Ui::init(Settings *settings, Engine *engine, Matrix *matrix, Display *display) {
	settings_ = settings;
	engine_ = engine;
	matrix_ = matrix;
	display_ = display;

	last_interval = 0;
	display_interval = 0;
	canvas_.init();
	ui_que.clear();

	pageManager_.init(settings, engine, &canvas_);
}

void Ui::poll() {
	uint8_t reading[16];
	matrix_->refresh(&reading[0]);

	for (int x = 0; x < 16; ++x) {
		for (int y = 0; y < 8; ++y) {
			uint8_t sw = y + (x * 8);
			sw_raw[sw] <<= 1;
			if (reading[x] & (1 << y)) {
				sw_raw[sw] |= 1;
			}
		}
	}

	for (int x = 0; x < 16 * 8; x += 8) {
		for (int y = 0; y < num_buttons_rows; ++y) {
			uint8_t button = buttons_rows[y] + x;
			if (sw_raw[button] == 0x80) {
				addEvent(Ui::BUTTON, button, 1);
			} else if (sw_raw[button] == 0x01) {
				addEvent(Ui::BUTTON, button, 0);
			}
		}

		for (int y = 0; y < num_enc_rows; ++y) {
			uint8_t a = enc_a_rows[y] + x;
			uint8_t b = enc_b_rows[y] + x;
			if ((sw_raw[b] & 0x03) == 0x02 && (sw_raw[a] & 0x03) == 0x00) {
				addEvent(Ui::ENCODER, a, 1);
			} else if ((sw_raw[a] & 0x03) == 0x02 && (sw_raw[b] & 0x03) == 0x00) {
				addEvent(Ui::ENCODER, a, -1);
			}
		}
	}
}

void Ui::process() {
	while (ui_que.readable()) {
		Ui::Event e = ui_que.read();
		switch (e.type)
		{
		case Ui::BUTTON:
			pageManager_.on_button(e.id, e.value);
			break;
		case Ui::ENCODER:
			pageManager_.on_encoder(e.id, e.value);
			break;
		default:
			break;
		}
	}

	uint32_t interval = (Micros::read() / 1000) - last_interval;

	if (interval >= 1) {
		last_interval += interval;
		matrix_->lock_leds();
		matrix_->clear_leds();
		pageManager_.draw_leds();
		matrix_->unlock_leds();
	}

	display_interval += interval;
	if (display_interval >= pageManager_.target_fps()) {
		send_display();
	}
}

void Ui::send_display() {
	while (display_->dma_busy());
	display_interval = 0;
	canvas_.clear();
	pageManager_.draw_display();
	display_->sendBuffer(canvas_.data(), canvas_.size());
}

void Ui::clear_que() {
	ui_que.clear();
}
