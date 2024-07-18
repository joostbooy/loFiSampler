#ifndef DelayList_h
#define DelayList_h

#include "settings.h"
#include "settingsText.h"

class DelayList : public SettingsList {

public:

	enum Item {
		AMOUNT,
		FEEDBACK,
		MIX,
		CHANNEL,

		NUM_ITEMS
	};


	const int num_items() override {
		return NUM_ITEMS;
	}

	const char* item_text(int item) override {
		switch (item)
		{
		case AMOUNT:	return "AMOUNT";
		case FEEDBACK:	return "FEEDBACK";
		case MIX:		return "MIX";
		case CHANNEL:	return "CHANNEL";
		default:
			break;
		}
		return nullptr;
	}

	const char* value_text(int item) override {
		Delay &delay = settings_->delay();

		switch (item)
		{
		case AMOUNT:	return delay.amount_text();
		case FEEDBACK:	return delay.feedback_text();
		case MIX:		return delay.mix_text();
		case CHANNEL:	return delay.channel_text();
		default:
			break;
		}
		return nullptr;
	}

	void edit(int item, int inc, bool shifted) override {
		Delay &delay = settings_->delay();

		switch (item)
		{
		case AMOUNT:	return delay.set_amount(delay.amount() + SettingsUtils::f_inc(inc, shifted));
		case FEEDBACK:	return delay.set_feedback(delay.feedback() + SettingsUtils::f_inc(inc, shifted));
		case MIX:		return delay.set_mix(delay.mix() + SettingsUtils::f_inc(inc, shifted));
		case CHANNEL:	return delay.set_channel(delay.channel() +inc);
		default:
			break;
		}
	}

private:

};

#endif
