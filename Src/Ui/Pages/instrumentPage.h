#ifndef InstrumentPage_h
#define InstrumentPage_h

#include "listPage.h"
#include "instrument.h"
#include "instrumentList.h"
#include "textInputPage.h"
#include "optionListPage.h"

namespace InstrumentPage {

	using TopPage::settings_;
	using TopPage::engine_;
	using TopPage::pages_;

	bool pasteable_;
	Instrument instrument_;
	InstrumentList instrumentList_;

	enum Options {
		EDIT_NAME,
		SAMPLE_LIST,
		MODULATION_MATRIX,
		CANCEL,
		NUM_OPTIONS
	};

	const char* const option_text[NUM_OPTIONS] = { "EDIT NAME", "SAMPLE LIST", "MODULATION MATRIX", "CANCEL" };

	void edit(int option) {
		switch (option)
		{
		case EDIT_NAME:
			TextInputPage::set(settings_->selected_instrument().name(), Instrument::kMaxNameLength, "SET INSTRUMENT NAME");
			pages_->open(Pages::TEXT_INPUT_PAGE);
			break;
		case SAMPLE_LIST:
			pages_->open(Pages::INSTRUMENT_SAMPLE_LIST_PAGE);
			break;
		case MODULATION_MATRIX:
			pages_->open(Pages::MODULATION_MATRIX_PAGE);
			break;
		default:
			break;
		}
	}

	void clear() {
		settings_->selected_instrument().init();
	}

	void copy() {
		instrument_.paste(&settings_->selected_instrument());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
			settings_->selected_instrument().paste(&instrument_);
			return true;
		}
		return false;
	}

	void init() {
		pasteable_ = false;
		instrument_.init();
		instrumentList_.init(engine_, settings_);
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

		if (state > 0 && id == Controller::MENU_BUTTON) {
			OptionListPage::set_count(NUM_OPTIONS);
			OptionListPage::set_text(option_text);
			OptionListPage::set_callback(edit);
			pages_->open(Pages::OPTION_LIST_PAGE);
		}
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
