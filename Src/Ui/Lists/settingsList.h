#ifndef SettingsList_h
#define SettingsList_h

#include "uiText.h"

class SettingsList {

public:

	int top_item() {
		return top_item_;
	}

	void select_item(int item) {
		top_item_ = stmlib::clip(0, num_items() - 1, item);
	}

	void on_up_button() {
		int value = top_item() - 4;
		if (value >= 0)  {
			select_item(value);
		}
	}

	void on_down_button() {
		int value = top_item() + 4;
		if (value < num_items())  {
			select_item(value);
		}
	}

	void on_encoder(int index, int inc, bool shifted) {
		edit(top_item() + index, inc, shifted);
	}

	virtual const int num_items();
	virtual const char* item_text(int item);
	virtual const char* value_text(int item);
	virtual void edit(int item, int inc, bool shifted);

private:
	int top_item_ = 0;
};

#endif
