#ifndef SamplePage_h
#define SamplePage_h

#include "listPage.h"
#include "sample.h"
#include "sampleList.h"

namespace SamplePage {

	bool pasteable_;
	Sample sample_;
	SampleList sampleList_;

	void clear() {
		TopPage::settings_->selected_sample().init();
	}

	void copy() {
		sample_.paste(&TopPage::settings_->selected_sample());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
			TopPage::settings_->selected_sample().paste(&sample_);
			return true;
		}
		return false;
	}

	void init() {
		pasteable_ = false;
		sample_.init();
		sampleList_.init(TopPage::engine_, TopPage::settings_);
	}

	void enter() {
		ListPage::set_list(&sampleList_);
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
