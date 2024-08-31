#ifndef WavImportPage_h
#define WavImportPage_h

#include "diskNavigatorPage.h"

namespace WavImportPage {

	using TopPage::pages_;
	using TopPage::settings_;

	enum Options {
		IMPORT,
		IMPORT_AS_MONO,
		IMPORT_ALL,
		IMPORT_ALL_AS_MONO,

		NUM_OPTIONS
	};

	StringBuilderBase<63>str_;

	const char* const option_text[NUM_OPTIONS] = { "IMPORT", "IMPORT AS MONO", "IMPORT ALL", "IMPORT ALL AS MONO" };

	void import_callback(int option) {
		int wavs_total_ = 0;
		int wavs_imported_ = 0;
		Disk *disk_ = settings_->disk();
		bool as_mono = option == IMPORT_AS_MONO || option == IMPORT_ALL_AS_MONO;

		switch (option)
		{
		case IMPORT:
		case IMPORT_AS_MONO:
			if (settings_->wavImporter().import(&disk_->file(), DiskNavigatorPage::curr_entry_path(), as_mono)) {
				MessagePainter::show("FINISHED");
			} else {
				MessagePainter::show("FAILED");
			}
			break;
		case IMPORT_ALL:
		case IMPORT_ALL_AS_MONO:
			disk_->entry().rewind();

			while (disk_->entry().next_visible()) {
				if (disk_->entry().is_dir() == false) {
					++wavs_total_;
					str_.write(DiskNavigatorPage::curr_path(), "/", disk_->entry().name());
					if (settings_->wavImporter().import(&disk_->file(), str_.read(), as_mono)) {
						++wavs_imported_;
					}
				}
			}

			MessagePainter::show(str_.write(wavs_imported_, "/", wavs_total_, " IMPORTED"));
			break;
		default:
			break;
		}
	}

	void confirm() {
		OptionListPage::set_text(option_text);
		OptionListPage::set_callback(import_callback);
		OptionListPage::set_count(NUM_OPTIONS);
		pages_->open(Pages::OPTION_LIST_PAGE);
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