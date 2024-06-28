#ifndef UiText_h
#define UiText_h

#include "stringBuilder.h"
#include "font.h"
#include "dsp.h"

class UiText {

public:

	static StringBuilderBase<31>str;

	static const char* int_to_text(int value) {
		return str.write(value);
	}

	static const char* float_to_text(float value, int min, int max) {
		return str.write(Dsp::cross_fade(min, max, value));
	}

	static const char* bool_to_on_off(bool state) {
		return state ? "ON" : "OFF";
	}

	static const char* bool_to_checkmark(bool state) {
		return state ? font.glyph(Font::CHECKMARK) : " ";
	}

	static const char* percentage_to_text(uint32_t ammount, uint32_t total) {
		float percentage = (100.f / total) * ammount;
		return str.write(static_cast<int>(percentage), "%");
	}

	static const char* note_to_text(uint8_t note, bool print_octave = true) {
		uint8_t oct = 0;
		while (note >= 12) {
			note -= 12;
			++oct;
		}

		str.write(note_text_[note]);

		if (print_octave) {
			str.append(oct);
		}

		return str.read();
	}
	static const char* midi_channel_text(int channel) {
		if (channel < 0) {
			return "NONE";
		} else if (channel >= 16) {
			return "OMNI";
		}

		return str.write(channel + 1);
	}

	static const char* kb_to_mem_size_text(uint32_t kb) {
		uint32_t gb = kb / 1000000;
		uint32_t mb = kb / 1000;

		if (gb > 0) {
			uint32_t gb_frac = (kb - (gb * 1000000)) / 10000;
			return str.write(gb, ".", gb_frac, " GB");
		} else if (mb > 0){
			uint32_t mb_frac = (kb - (mb * 1000)) / 10;
			return str.write(mb, ".", mb_frac, " MB");
		} else {
			return str.write(kb, " KB");
		}
	}

private:
	static const char* note_text_[12];
};

#endif
