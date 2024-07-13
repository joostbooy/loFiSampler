#ifndef Leds_h
#define Leds_h

class Leds {

public:

	enum Color {
		BLACK,
		RED
	};

	void init() {
		clear();
	}

	uint8_t* data() {
		return &data_[0];
	}

	void clear() {
		for (size_t i = 0; i < kNumOfCollumns; ++i) {
			data_[i] = 0;
		}
	}

	void set_all(Color color) {
		footer_buttons(color, color, color, color);
		footer_encoders(color, color, color, color);
		for (int i = 0; i < 6; ++i) {
			set(chapter_leds_[i].x, chapter_leds_[i].y, color);
		}
	}

	// chapter
	void set_chapter(int index) {
		for (int i = 0; i < 6; ++i) {
			Color color = i == index ? RED : BLACK;
			set(chapter_leds_[i].x, chapter_leds_[i].y, color);
		}
	}

	// footer buttons
	void set_footer_button(int index, Color color) {
		set(footer_button_leds_[index].x, footer_button_leds_[index].y, color);
	}
	
	void footer_buttons(Color c1, Color c2, Color c3, Color c4) {
		Color colors[4] = {c1, c2, c3, c4};
		for (int i = 0; i < 4; ++i) {
			set_footer_button(i, colors[i]);
		}
	}

	// footer encoders
	void set_footer_encoder(int index, Color color) {
		set(footer_encoder_leds_[index].x, footer_encoder_leds_[index].y, color);
	}

	void footer_encoders(Color c1, Color c2, Color c3, Color c4) {
		Color colors[4] = {c1, c2, c3, c4};
		for (int i = 0; i < 4; ++i) {
			set_footer_encoder(i, colors[i]);
		}
	}

private:
	static const size_t kNumOfCollumns = 7;
	uint8_t data_[kNumOfCollumns];

	void set(int x, int y, Color color) {
		uint8_t value = data_[x] & ~(1 << y);
		data_[x] = value | (color << y);
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
