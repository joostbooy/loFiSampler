#ifndef MidiPage_h
#define MidiPage_h

#include "listPage.h"
#include "midi.h"
#include "midiList.h"

namespace MidiPage {

	bool pasteable_;
	Midi midi_;
	MidiList midiList_;

	void clear() {
		TopPage::settings_->midi().init();
	}

	void copy() {
		midi_.paste(&TopPage::settings_->midi());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
			TopPage::settings_->midi().paste(&midi_);
			return true;
		}
		return false;
	}

	void init() {
		pasteable_ = false;
		midi_.init();
		midiList_.init(TopPage::engine_, TopPage::settings_);
	}

	void enter() {
		ListPage::set_list(&midiList_);
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
		ListPage::on_encoder(id, state);
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
