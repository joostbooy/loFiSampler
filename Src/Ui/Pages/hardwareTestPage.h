#ifndef HardwareTestPage_h
#define HardwareTestPage_h

#include "controller.h"
#include "textBufferPainter.h"

namespace HardwareTestPage {

	const char *id_text(int id) {
		switch (id)
		{
		case Controller::FUNCTION_BUTTON_A:			return "FUNCTION BUTTON A";
		case Controller::FUNCTION_BUTTON_B:			return "FUNCTION BUTTON B";
		case Controller::FUNCTION_BUTTON_C:			return "FUNCTION BUTTON C";
		case Controller::FUNCTION_BUTTON_D:			return "FUNCTION BUTTON D";
		case Controller::FUNCTION_ENC_A:			return "FUNCTION ENC A";
		case Controller::FUNCTION_ENC_B:			return "FUNCTION ENC B";
		case Controller::FUNCTION_ENC_C:			return "FUNCTION ENC C";
		case Controller::FUNCTION_ENC_D:			return "FUNCTION ENC D";
		case Controller::FUNCTION_ENC_PUSH_A:		return "FUNCTION ENC PUSH A";
		case Controller::FUNCTION_ENC_PUSH_B:		return "FUNCTION ENC_PUSH B";
		case Controller::FUNCTION_ENC_PUSH_C:		return "FUNCTION ENC_PUSH C";
		case Controller::FUNCTION_ENC_PUSH_D:		return "FUNCTION ENC PUSH D";
		case Controller::UP_BUTTON:					return "UP BUTTON";
		case Controller::DOWN_BUTTON:				return "DOWN BUTTON";
		case Controller::LEFT_BUTTON:				return "LEFT BUTTON";
		case Controller::RIGHT_BUTTON:				return "RIGHT BUTTON";
		case Controller::SHIFT_BUTTON:				return "SHIFT BUTTON";
		case Controller::COPY_BUTTON:				return "COPY BUTTON";
		case Controller::PASTE_BUTTON:				return "PASTE BUTTON";
		case Controller::CLEAR_BUTTON:				return "CLEAR BUTTON";
		case Controller::MENU_BUTTON:				return "MENU BUTTON";
		case Controller::SAMPLE_CHAPTER_BUTTON:		return "SAMPLE CHAPTER BUTTON";
		case Controller::INSTRUMENT_CHAPTER_BUTTON:	return "INSTRUMENT CHAPTER BUTTON";
		case Controller::MODULATION_CHAPTER_BUTTON:	return "MODULATION CHAPTER BUTTON";
		case Controller::SYSTEM_CHAPTER_BUTTON:		return "SYSTEM CHAPTER BUTTON";
		case Controller::MIDI_CHAPTER_BUTTON:		return "MIDI CHAPTER BUTTON";
		case Controller::STORAGE_CHAPTER_BUTTON:	return "STORAGE CHAPTER BUTTON";
		default:									return "";
			break;
		}
		return "";
	}

	enum FooterOptions {
		TOGGLE_LEDS,
		NUM_OPTIONS
	};

	const char* const footer_option_text[NUM_OPTIONS] = { "TOGGLE LEDS" };

	bool led_toggle_state_;

	void init() {

	}

	void enter() {

	}

	void exit()  {

	}


	void on_button(int id, int state) {
		TextBufferPainter::write(TopPage::str_.write(id_text(id), " ", state));

		switch (Controller::button_to_function(id))
		{
		case TOGGLE_LEDS:
			if (state) {
				if (led_toggle_state_) {
					led_toggle_state_ = 0;
					LedPainter::set_all(Matrix::BLACK);
				} else {
					led_toggle_state_ = 1;
					LedPainter::set_all(Matrix::RED);
				}
			}
			break;
		default:
			/* code */
			break;
		}
	}

	void on_encoder(int id, int state) {
		TextBufferPainter::write(TopPage::str_.write(id_text(id), " ", state));
	}

	void refresh_leds() {

	}

	void draw() {
		TextBufferPainter::draw();
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
