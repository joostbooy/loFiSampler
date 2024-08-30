#ifndef WavImportPage_h
#define WavImportPage_h

#include "diskNavigatorPage.h"

namespace WavImportPage {

	using TopPage::pages_;
	using TopPage::settings_;

	enum Options {
		IMPORT,
		IMPORT_AS_MONO,
		//	IMPORT_ALL,
		//	IMPORT_ALL_AS_MONO,

		NUM_OPTIONS
	};

	const char* const option_text[NUM_OPTIONS] = { "IMPORT", "IMPORT AS MONO" };//, "IMPORT ALL", "IMPORT ALL AS MONO" };

	void import_callback(int option) {
		switch (option)
		{
		case IMPORT:
			if (settings_->wavImporter().import(&settings_->disk()->file(), DiskNavigatorPage::curr_entry_path(), false)) {
				MessagePainter::show("FINISHED");
				return;
			}
			break;
		case IMPORT_AS_MONO:
			if (settings_->wavImporter().import(&settings_->disk()->file(), DiskNavigatorPage::curr_entry_path(), true)) {
				MessagePainter::show("FINISHED");
				return;
			}
			break;
		default:
			break;
		}

		MessagePainter::show("FAILED");
	}

	void confirm() {
		OptionListPage::set_text(option_text);
		OptionListPage::set_callback(import_callback);
		OptionListPage::set_count(NUM_OPTIONS);
	}

	void init() {
		DiskNavigatorPage::init();
	}

	void enter() {
		DiskNavigatorPage::set_callback(confirm);
		DiskNavigatorPage::set_list_filter(Entry::WAV);
		DiskNavigatorPage::enter();
	}

	void exit()  {
		DiskNavigatorPage::exit();
	}

	void on_button(int id, int state) {
		DiskNavigatorPage::on_button(id, state);
	}

	void on_encoder(int id, int state) {
		DiskNavigatorPage::on_encoder(id, state);
	}

	void refresh_leds() {
		DiskNavigatorPage::refresh_leds();
	}

	void draw() {
		DiskNavigatorPage::draw();
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
