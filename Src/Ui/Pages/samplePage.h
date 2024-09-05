#ifndef SamplePage_h
#define SamplePage_h

#include "listPage.h"
#include "sample.h"
#include "sampleList.h"

namespace SamplePage {

	using TopPage::settings_;
	using TopPage::engine_;
	using TopPage::pages_;
	using TopPage::canvas_;

	bool pasteable_;
	Sample sample_;
	SampleList sampleList_;

	int selected_option_;

	enum ListOptions {
		IMPORT,
		REMOVE,
		CONVERT_TO_MONO,
		MAP_NAME_TO_ROOT_NOTE,
		MAP_NAME_TO_KEY_RANGE,
		MAP_NAME_TO_ROOT_AND_KEY,
		CANCEL,

		NUM_LIST_OPTIONS
	};

	const char* const list_option_text[NUM_LIST_OPTIONS] = {
		"IMPORT",
		"REMOVE",
		"CONVERT TO MONO",
		"MAP NAME TO ROOT NOTE",
		"MAP NAME TO KEY RANGE",
		"MAP NAME TO ROOT & KEY",
		"CANCEL"
	};

	void clear() {
		settings_->selected_sample()->init();
	}

	void copy() {
		sample_.paste(settings_->selected_sample());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
			settings_->selected_sample()->paste(&sample_);
			return true;
		}
		return false;
	}

	void remove(int index) {
		engine_->add_request_blocking(Engine::STOP);

		if (settings_->sampleAllocator().remove(index)) {
			settings_->refresh_sample_selection();
			MessagePainter::show("SAMPLE REMOVED");
		}
	}

	void remove_all() {
		engine_->add_request_blocking(Engine::STOP);

		size_t count = settings_->num_samples();
		for (size_t i = 0; i < count; ++i) {
			settings_->sampleAllocator().remove(i);
		}

		settings_->refresh_sample_selection();
		MessagePainter::show("SAMPLES REMOVED");
	}

	void convert_to_mono(int index) {
		engine_->add_request_blocking(Engine::STOP);

		if (settings_->sampleAllocator().convert_to_mono(index)) {
			MessagePainter::show("SAMPLE CONVERTED TO MONO");
		}
	}

	void convert_all_to_mono() {
		engine_->add_request_blocking(Engine::STOP);

		size_t count = settings_->num_samples();
		for (size_t i = 0; i < count; ++i) {
			settings_->sampleAllocator().convert_to_mono(i);
		}
		MessagePainter::show("SAMPLES CONVERTED TO MONO");
	}

	void map_name_to_root_note(int index) {
		Sample *sample = settings_->sample(index);
		if (sample->map_name_to_root_note()) {
			MessagePainter::show("SUCCES");
		}
	}

	void map_all_names_to_root_note() {
		size_t count = settings_->num_samples();
		for (size_t i = 0; i < count; ++i) {
			settings_->sample(i)->map_name_to_root_note();
		}
		MessagePainter::show("FINISHED");
	}

	void map_name_to_key_range(int index) {
		Sample *sample = settings_->sample(index);
		if (sample->map_name_to_key_range()) {
			MessagePainter::show("SUCCES");
		}
	}

	void map_all_names_to_key_range() {
		size_t count = settings_->num_samples();
		for (size_t i = 0; i < count; ++i) {
			settings_->sample(i)->map_name_to_key_range();
		}
		MessagePainter::show("FINISHED");
	}

	void edit(int option) {
		if (option != ConfirmationPage::CANCEL) {
			return;
		}

		bool apply_to_all = option == ConfirmationPage::APPLY_TO_ALL;

		switch (selected_option_)
		{
		case REMOVE:
			if (apply_to_all) {
				remove_all();
			} else {
				remove(settings_->selected_sample_index());
			}
			break;
		case CONVERT_TO_MONO:
			if (apply_to_all) {
				convert_all_to_mono();
			} else {
				convert_to_mono(settings_->selected_sample_index());
			}
			break;
		case MAP_NAME_TO_ROOT_NOTE:
			if (apply_to_all) {
				map_all_names_to_root_note();
			} else {
				map_name_to_root_note(settings_->selected_sample_index());
			}
			break;
		case MAP_NAME_TO_KEY_RANGE:
			if (apply_to_all) {
				map_all_names_to_key_range();
			} else {
				map_name_to_key_range(settings_->selected_sample_index());
			}
			break;
		case MAP_NAME_TO_ROOT_AND_KEY:
			if (apply_to_all) {
				map_all_names_to_root_note();
				map_all_names_to_key_range();
			} else {
				map_name_to_root_note(settings_->selected_sample_index());
				map_name_to_key_range(settings_->selected_sample_index());
			}
			break;
		case CANCEL:
			break;
		default:
			break;
		}
	}

	void select_edit_option(int option) {
		if (option == IMPORT) {
			pages_->open(Pages::WAV_IMPORT_PAGE);
		} else if (option != CANCEL) {
			selected_option_ = option;
			ConfirmationPage::set("CONTINUE ?", edit, true);
			pages_->open(Pages::CONFIRMATION_PAGE);
		}
	}

	void init() {
		pasteable_ = false;
		sample_.init();
		sampleList_.init(engine_, settings_);
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

		if (state) {
			if (id == Controller::MENU_BUTTON) {
				OptionListPage::set_text(list_option_text);
				OptionListPage::set_callback(select_edit_option);
				OptionListPage::set_count(NUM_LIST_OPTIONS);
				pages_->open(Pages::OPTION_LIST_PAGE);
				return;
			}
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

		if (settings_->num_samples() == 0) {
			return;
		}

		const int x = 2;
		const int y = 5;
		const int w = 251;
		const int h = 32;

		size_t index = 0;
		int16_t left = 0;
		int16_t right = 0;

		Sample *sample = settings_->selected_sample();
		size_t size = sample->size_samples();
		size_t inc = size / w;

		for (int x2 = 0; x2 < w; ++x2) {
			sample->read(index, &left, &right);

			float left_ = left * (1.f / 32767.f);
			float right_ = right * (1.f / 32767.f);
			float mix = (left_ +  right_) * 0.5f;
			int y2 = h * (1.f - mix);

			canvas_->draw_pixel(x + x2, y + y2, Canvas::BLACK);
			index += inc;
		}

		float start = sample->start() / size;
		canvas_->vertical_line(start * w, y, h, Canvas::INVERTED);

		float end = sample->end() / size;
		canvas_->vertical_line(end * w, y, h, Canvas::INVERTED);

		float loop_start = sample->loop_start() / size;
		canvas_->vertical_line(loop_start * w, y, h, Canvas::INVERTED);

		float loop_end = sample->loop_end() / size;
		canvas_->vertical_line(loop_end * w, y, h, Canvas::INVERTED);
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
