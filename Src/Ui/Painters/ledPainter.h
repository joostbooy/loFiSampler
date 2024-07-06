#ifndef LedPainter_h
#define LedPainter_h

#include "ui.h"
#include "matrix.h"
#include "engine.h"

class LedPainter {

public:

	static void init(Matrix *matrix) {
		matrix_ = matrix;
	}

	static void set_all(Matrix::LedColor color) {
		if (color == Matrix::BLACK) {
			matrix_->clear_leds();
		} else {
			footer_buttons(Matrix::RED, Matrix::RED, Matrix::RED, Matrix::RED);
			footer_encoders(Matrix::RED, Matrix::RED, Matrix::RED, Matrix::RED);
			for (int i = 0; i < 6; ++i) {
				matrix_->set_led(chapter_leds_[i].x, chapter_leds_[i].y, Matrix::RED);
			}
		}
	}

	// chapter
	static void set_chapter(int index) {
		for (int i = 0; i < 6; ++i) {
			Matrix::LedColor color = i == index ? Matrix::RED : Matrix::BLACK;
			matrix_->set_led(chapter_leds_[i].x, chapter_leds_[i].y, color);
		}
	}

	// footer buttons
	static void set_footer_button(int index, Matrix::LedColor color) {
		matrix_->set_led(footer_button_leds_[index].x, footer_button_leds_[index].y, color);
	}

	static void footer_buttons(int num_active) {
		for (int i = 0; i < 4; ++i) {
			Matrix::LedColor color = i < num_active ? Matrix::RED : Matrix::BLACK;
			set_footer_button(i, color);
		}
	}

	static void footer_buttons(Matrix::LedColor c1, Matrix::LedColor c2, Matrix::LedColor c3, Matrix::LedColor c4) {
		Matrix::LedColor colors[4] = {c1, c2, c3, c4};
		for (int i = 0; i < 4; ++i) {
			set_footer_button(i, colors[i]);
		}
	}

	// footer encoders
	static void set_footer_encoder(int index, Matrix::LedColor color) {
		matrix_->set_led(footer_encoder_leds_[index].x, footer_encoder_leds_[index].y, color);
	}

	static void footer_encoders(int num_active) {
		for (int i = 0; i < 4; ++i) {
			Matrix::LedColor color = i < num_active ? Matrix::RED : Matrix::BLACK;
			set_footer_encoder(i, color);
		}
	}

	static void footer_encoders(Matrix::LedColor c1, Matrix::LedColor c2, Matrix::LedColor c3, Matrix::LedColor c4) {
		Matrix::LedColor colors[4] = {c1, c2, c3, c4};
		for (int i = 0; i < 4; ++i) {
			set_footer_encoder(i, colors[i]);
		}
	}

private:
	static Matrix *matrix_;

	struct Map {
		const uint8_t x;
		const uint8_t y;
	};

	static Map chapter_leds_[6];
	static Map footer_button_leds_[4];
	static Map footer_encoder_leds_[4];
};

#endif
