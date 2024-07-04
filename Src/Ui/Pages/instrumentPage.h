#ifndef InstrumentPage_h
#define InstrumentPage_h

#include "listPage.h"
#include "instrument.h"
#include "instrumentList.h"

namespace InstrumentPage {

	bool pasteable_;
	Instrument instrument_;
	EnvelopeList instrumentList_;

	void clear() {
		TopPage::settings_->selected_instrument().init();
	}

	void copy() {
		instrument_.paste(&TopPage::settings_->selected_instrument());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
			TopPage::settings_->selected_instrument().paste(&instrument_);
			return true;
		}
		return false;
	}

	void init() {
		pasteable_ = false;
		instrument_.init();
		instrumentList_.init(TopPage::engine_, TopPage::settings_);
	}

	void enter() {
		ListPage::set_list(&instrumentList_);
		ListPage::set_clear_callback(&clear);
		ListPage::set_copy_callback(&copy);
		ListPage::set_paste_callback(&paste);
		ListPage::enter();
	}

	void exit()  {
		ListPage::exit();
	}

	void on_button(int id, int state) {
		ListPage::on_button(id, state);
	}

	void on_encoder(int id, int state) {
		ListPage::on_button(id, state);
	}

	void refresh_leds() {
		ListPage::refresh_leds();
	}

	void draw() {
		ListPage::draw();
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
