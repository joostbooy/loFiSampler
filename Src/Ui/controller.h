#ifndef Controller_h
#define Controller_h

#include "stmf4lib.h"

#define SERIAL_ID(collumn, row) ((collumn * 8) + row)

class Controller {

public:

	enum Id {
		FUNCTION_BUTTON_A			= SERIAL_ID(0, 4),
		FUNCTION_BUTTON_B			= SERIAL_ID(2, 4),
		FUNCTION_BUTTON_C			= SERIAL_ID(4, 4),
		FUNCTION_BUTTON_D			= SERIAL_ID(6, 4),
		FUNCTION_ENC_A				= SERIAL_ID(0, 4),
		FUNCTION_ENC_B				= SERIAL_ID(2, 4),
		FUNCTION_ENC_C				= SERIAL_ID(4, 4),
		FUNCTION_ENC_D				= SERIAL_ID(6, 4),
		FUNCTION_ENC_PUSH_A			= SERIAL_ID(0, 4),
		FUNCTION_ENC_PUSH_B			= SERIAL_ID(2, 4),
		FUNCTION_ENC_PUSH_C			= SERIAL_ID(4, 4),
		FUNCTION_ENC_PUSH_D			= SERIAL_ID(6, 4),
		UP_BUTTON					= SERIAL_ID(7, 4),
		DOWN_BUTTON					= SERIAL_ID(9, 4),
		LEFT_BUTTON					= SERIAL_ID(11, 4),
		RIGHT_BUTTON				= SERIAL_ID(13, 4),
		SHIFT_BUTTON				= SERIAL_ID(0, 3),
		COPY_BUTTON					= SERIAL_ID(15, 3),
		PASTE_BUTTON				= SERIAL_ID(15, 0),
		CLEAR_BUTTON				= SERIAL_ID(13, 3),
		ENTER_BUTTON				= SERIAL_ID(13, 3),
		SAMPLE_CHAPTER_BUTTON		= SERIAL_ID(12, 0),
		INSTRUMENT_CHAPTER_BUTTON	= SERIAL_ID(10, 0),
		SYSTEM_CHAPTER_BUTTON		= SERIAL_ID(10, 3),
		MODULATION_CHAPTER_BUTTON	= SERIAL_ID(5, 0),
	};

	static void init() {
		prev_touched_encoder_ = 0;
		curr_touched_encoder_ = 0;
		for (int i = 0; i < (8 * 8) / 32; ++i) {
			pressed_[i] = 0;
		}
	}

	static uint8_t curr_touched_encoder() {
		return curr_touched_encoder_;
	}

	static uint8_t prev_touched_encoder() {
		return prev_touched_encoder_;
	}

	static void on_encoder(uint8_t id) {
		if (id != curr_touched_encoder_) {
			prev_touched_encoder_ = curr_touched_encoder_;
			curr_touched_encoder_ = id;
		}
	}

	static void on_button(uint8_t id, bool state) {
		uint8_t index = id / 32;
		uint32_t mask = 1 << (id & 31);
		pressed_[index] &= ~(mask);
		if (state) {
			pressed_[index] |= mask;
		}
	}

	static bool is_pressed(uint8_t id) {
		uint8_t index = id / 32;
		uint32_t mask = 1 << (id & 31);
		return pressed_[index] & mask;
	}

	static int8_t button_to_function(uint8_t id) {
		switch (id)
		{
		case FUNCTION_BUTTON_A:		return 0;
		case FUNCTION_BUTTON_B:		return 1;
		case FUNCTION_BUTTON_C:		return 2;
		case FUNCTION_BUTTON_D:		return 3;
		default:					return -1;
			break;
		}
	}

	static int8_t encoder_to_function(uint8_t id) {
		switch (id)
		{
		case FUNCTION_ENC_A:	return 0;
		case FUNCTION_ENC_B:	return 1;
		case FUNCTION_ENC_C:	return 2;
		case FUNCTION_ENC_D:	return 3;
		default:				return -1;
			break;
		}
	}

	static bool encoder_is_pressed(uint8_t encoder_id) {
		switch (encoder_id)
		{
		case FUNCTION_ENC_A:	return is_pressed(FUNCTION_ENC_PUSH_A);
		case FUNCTION_ENC_B:	return is_pressed(FUNCTION_ENC_PUSH_B);
		case FUNCTION_ENC_C:	return is_pressed(FUNCTION_ENC_PUSH_C);
		case FUNCTION_ENC_D:	return is_pressed(FUNCTION_ENC_PUSH_D);
		default:			return false;
			break;
		}
	}

private:
	static uint32_t pressed_[(8 * 8) / 32];
	static uint8_t curr_touched_encoder_;
	static uint8_t prev_touched_encoder_;
};

#endif
