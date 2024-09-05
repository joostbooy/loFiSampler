#ifndef ProjectPage_h
#define ProjectPage_h

#include "diskNavigatorPage.h"

namespace ProjectPage {

	using TopPage::pages_;
	using TopPage::settings_;
	using TopPage::canvas_;
	using TopPage::ui_;

	enum Options {
		LOAD,
		SAVE,
		SAVE_AS,

		NUM_OPTIONS
	};

	const char* const option_text[NUM_OPTIONS] = { "LOAD", "SAVE", "SAVE AS" };

	void save(bool confirmed) {
		if (confirmed) {
			if (settings_->save(DiskNavigatorPage::curr_path())) {
				settings_->set_project_name(DiskNavigatorPage::curr_entry_name());
				MessagePainter::show("PROJECT SAVED");
			} else {
				MessagePainter::show("SAVE FAILED !");
			}
		}
	}

	void option_callback(int option) {
		switch (option)
		{
		case LOAD:
			ConfirmationPage::set("LOAD PROJECT ?", [](int option) {
				if (option == ConfirmationPage::CONFIRM) {
					if (settings_->load(DiskNavigatorPage::curr_entry_path())) {
						MessagePainter::show("PROJECT LOADED");
					} else {
						MessagePainter::show("LOAD FAILED !");
					}
				}
			});
			pages_->open(Pages::CONFIRMATION_PAGE);
			break;
		case SAVE:
		case SAVE_AS:
			if (settings_->project_name() == nullptr || option == SAVE_AS) {
				TextInputPage::set(settings_->project_name(), settings_->max_name_length(), "SET PROJECT NAME", &save);
				pages_->open(Pages::TEXT_INPUT_PAGE);
			} else {
				save(true);
			}
			break;
		default:
			break;
		}
	}

	void init() {
		DiskNavigatorPage::init();
	}

	void enter() {
		DiskNavigatorPage::set_footer_text(option_text);
		DiskNavigatorPage::set_num_footer_options(NUM_OPTIONS);
		DiskNavigatorPage::set_footer_callback(option_callback);
		DiskNavigatorPage::set_list_filter(Entry::PROJECT);
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
