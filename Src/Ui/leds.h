#ifndef Leds_h
#define Leds_h

class Leds {

public:

	enum Color {
		BLACK,
		RED
	};

	void init() {
		set_all(BLACK);
	}

	uint8_t* data() {
		return &data_[0];
	}

	void set_all(Color color) {
		uint8_t value = color == RED ? 0xFF : 0x00;
		for (size_t i = 0; i < kNumOfCollumns; ++i) {
			data_[i] = value;
		}
	}

	// Chapter
	void set_chapter(int index) {
		for (int i = 0; i < 6; ++i) {
			Color color = i == index ? RED : BLACK;
			set(chapter_leds_[i].x, chapter_leds_[i].y, color);
		}
	}

	// Footer buttons
	void set_footer_button(int index, Color color) {
		set(footer_button_leds_[index].x, footer_button_leds_[index].y, color);
	}

	void set_footer_buttons(int num_active) {
		for (int i = 0; i < 4; ++i) {
			set_footer_button(i, i < num_active ? RED : BLACK);
		}
	}

	// Footer encoders
	void set_footer_encoder(int index, Color color) {
		set(footer_encoder_leds_[index].x, footer_encoder_leds_[index].y, color);
	}

	void set_footer_encoders(int num_active) {
		for (int i = 0; i < 4; ++i) {
			set_footer_encoder(i, i < num_active ? RED : BLACK);
		}
	}

private:
	static const size_t kNumOfCollumns = 7;
	uint8_t data_[kNumOfCollumns];

	void set(int x, int y, Color color) {
		//int y_ = y + 4;
		int y_ = y;
		uint8_t value = data_[x] & ~(1 << y_);
		data_[x] = value | (color << y_);
	}

	struct Map {
		const uint8_t x;
		const uint8_t y;
	};

	Map footer_button_leds_[4] = {
		{ .x = 3, .y = 3 },
		{ .x = 4, .y = 3 },
		{ .x = 5, .y = 3 },
		{ .x = 6, .y = 3 },
	};

	Map footer_encoder_leds_[4] = {
		{ .x = 3, .y = 2 },
		{ .x = 4, .y = 2 },
		{ .x = 5, .y = 2 },
		{ .x = 6, .y = 2 },
	};

	Map chapter_leds_[6] = {
		{ .x = 1, .y = 0 },	// Sample
		{ .x = 0, .y = 0 },	// Instrument
		{ .x = 1, .y = 1 },	// Modulation
		{ .x = 2, .y = 0 },	// System
		{ .x = 2, .y = 1 },	// Storage
		{ .x = 0, .y = 1 },	// Midi
	};

};

#endif
