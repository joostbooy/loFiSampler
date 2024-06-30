#include "controller.h"

Controller controller;

void Controller::init() {
	for (int i = 0; i < (8 * 8) / 32; ++i) {
		pressed_[i] = 0;
	}
}

void Controller::on_button(uint8_t id, bool state) {
	uint8_t index = id / 32;
	uint32_t mask = 1 << (id & 31);
	pressed_[index] &= ~(mask);
	if (state) {
		pressed_[index] |= mask;
	}
}

bool Controller::is_pressed(uint8_t id) {
	uint8_t index = id / 32;
	uint32_t mask = 1 << (id & 31);
	return pressed_[index] & mask;
}
