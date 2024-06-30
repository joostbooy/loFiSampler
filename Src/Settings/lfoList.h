#ifndef LfoList_h
#define LfoList_h

#include "uiText.h"
#include "settings.h"

class LfoList : public SettingsList {

public:

	enum Item {
		INDEX,
		SPEED,
		SHAPE,
		SKEW,
		RANDOMISE,
		CLOCK_SYNC,
		RETRIGGER_PORT,
		RETRIGGER_CHANNEL,

		NUM_ITEMS
	};


	const int num_items() override {
		return NUM_ITEMS;
	}

	const char* item_text(int item) override {
		switch (item)
		{
		case INDEX:				return "LFO";
		case SPEED:				return "SPEED";
		case SHAPE:				return "SHAPE";
		case SKEW:				return "SKEW";
		case RANDOMISE:			return "RANDOMISE";
		case CLOCK_SYNC:		return "CLOCK SYNC";
		case RETRIGGER_PORT:	return "RETRIGGER PORT";
		case RETRIGGER_CHANNEL:	return "RETRIGGER CHANNEL";
		default:
			break;
		}
		return nullptr;
	}

	const char* value_text(int item) override {
		Lfo &lfo = settings.selected_lfo();

		switch (item)
		{
		case INDEX:				return UiText::int_to_text(settings.selected_lfo_index() + 1);
		case SPEED:				return lfo.speed_text();
		case SHAPE:				return lfo.shape_text();
		case SKEW:				return lfo.skew_text();
		case RANDOMISE:			return lfo.randomise_text();
		case CLOCK_SYNC:		return lfo.clock_sync_text();
		case RETRIGGER_PORT:	return lfo.retrigger_port_text();
		case RETRIGGER_CHANNEL:	return lfo.retrigger_channel_text();
		default:
			break;
		}
		return nullptr;
	}

	void edit(int item, int inc, bool shifted) override {
		Lfo &lfo = settings.selected_lfo();

		switch (item)
		{
		case INDEX:
			settings.select_lfo_index(settings.selected_lfo_index() + inc);
			break;
		case SPEED:
			lfo.set_speed(lfo.speed() + f_inc(inc, shifted));
			break;
		case SHAPE:
			lfo.set_shape(lfo.shape() + f_inc(inc, shifted));
			break;
		case SKEW:
			lfo.set_skew(lfo.skew() + f_inc(inc, shifted));
			break;
		case RANDOMISE:
			lfo.set_randomise(inc > 0);
			break;
		case CLOCK_SYNC:
			lfo.set_clock_sync(inc > 0);
			break;
		case RETRIGGER_PORT:
			lfo.set_retrigger_port(lfo.retrigger_port() + inc);
			break;
		case RETRIGGER_CHANNEL:
			lfo.set_retrigger_channel(lfo.retrigger_channel() + inc);
			break;
		default:
			break;
		}
	}

private:

	inline float f_inc(int inc, bool shift) {
		return inc * (1.f / 100.f) * (shift ? 10.f : 1.f);
	}
};

#endif
