#ifndef Controller_h
#define Controller_h

#include "stmf4lib.h"

#define SERIAL_ID(collumn, row) ((collumn * 8) + row)

class Controller {

public:

	enum Id {
		FUNCTION_ENC_A				= SERIAL_ID(0, 4),
		FUNCTION_ENC_B				= SERIAL_ID(2, 4),
		FUNCTION_ENC_C				= SERIAL_ID(4, 4),
		FUNCTION_ENC_D				= SERIAL_ID(6, 4),
		FUNCTION_ENC_A_PUSH			= SERIAL_ID(0, 4),
		FUNCTION_ENC_B_PUSH			= SERIAL_ID(2, 4),
		FUNCTION_ENC_C_PUSH			= SERIAL_ID(4, 4),
		FUNCTION_ENC_D_PUSH			= SERIAL_ID(6, 4),
		FUNCTION_BUTTON_A			= SERIAL_ID(0, 4),
		FUNCTION_BUTTON_B			= SERIAL_ID(2, 4),
		FUNCTION_BUTTON_C			= SERIAL_ID(4, 4),
		FUNCTION_BUTTON_D			= SERIAL_ID(6, 4),
		UP_BUTTON					= SERIAL_ID(8, 4),
		DOWN_BUTTON					= SERIAL_ID(10, 4),
		LEFT_BUTTON					= SERIAL_ID(12, 4),
		RIGHT_BUTTON				= SERIAL_ID(14, 4),
		SHIFT_BUTTON				= SERIAL_ID(0, 3),
		COPY_BUTTON					= SERIAL_ID(15, 3),
		PASTE_BUTTON				= SERIAL_ID(15, 0),
		CLEAR_BUTTON				= SERIAL_ID(13, 3),
		MODULATION_CHAPTER_BUTTON	= SERIAL_ID(10, 0),
		INSTRUMENT_CHAPTER_BUTTON	= SERIAL_ID(10, 3),
		SAMPLE_CHAPTER_BUTTON		= SERIAL_ID(5, 0),
		SYSTEM_CHAPTER_BUTTON		= SERIAL_ID(7, 3),
	};

	void init();
	void on_button(uint8_t id, bool state);
	bool is_pressed(uint8_t id);

	int8_t button_to_function(uint8_t id) {
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

	int8_t encoder_to_function(uint8_t id) {
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

	bool encoder_is_pressed(uint8_t encoder_id) {
		switch (encoder_id)
		{
		case FUNCTION_ENC_A:	return is_pressed(FUNCTION_ENC_A_PUSH);
		case FUNCTION_ENC_B:	return is_pressed(FUNCTION_ENC_B_PUSH);
		case FUNCTION_ENC_C:	return is_pressed(FUNCTION_ENC_C_PUSH);
		case FUNCTION_ENC_D:	return is_pressed(FUNCTION_ENC_D_PUSH);
		default:			return false;
			break;
		}
	}

private:
	uint32_t pressed_[(8 * 8) / 32];
};

extern Controller controller;

#endif
