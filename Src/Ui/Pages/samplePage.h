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
	using TopPage::str_;

	bool pasteable_;
	Sample sample_;
	SampleList sampleList_;

	int zoom_ = 1;
	int sample_x_ = 0;
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
		if (option == ConfirmationPage::CANCEL) {
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
		if (settings_->num_samples() == 0) {
			pages_->close(Pages::SAMPLE_PAGE);
			pages_->open(Pages::WAV_IMPORT_PAGE);
		} else {
			ListPage::set_list(&sampleList_);
			ListPage::set_clear_callback(&clear);
			ListPage::set_copy_callback(&copy);
			ListPage::set_paste_callback(&paste);
			ListPage::enter();
		}
	}

	void exit()  {
		ListPage::exit();
	}

	void on_encoder(int id, int state) {
		if (Controller::is_pressed(Controller::SHIFT_BUTTON)) {
			int inc = Controller::encoder_is_pressed(id) ? state * 10 : state * 1;
			sample_x_ = SettingsUtils::clip(0, 251, sample_x_ + inc);
			return;
		}

		//	const int kMilliSecond = 1000 / SAMPLE_RATE;
		//	const int kQuarterSecond = SAMPLE_RATE / 4;

		//	if (Controller::is_pressed(Controller::SHIFT_BUTTON)) {
		//		int inc = Controller::encoder_is_pressed(id) ? kQuarterSecond * state : kMilliSecond * state;
		//		sample_x_ = SettingsUtils::clip(0, settings_->selected_sample()->size_samples(), sample_x_ + inc);
		//		return;
		//	}

		ListPage::on_encoder(id, state);
	}

	void on_button(int id, int state) {
		bool shift = Controller::is_pressed(Controller::SHIFT_BUTTON);

		if (!shift) {
			ListPage::on_button(id, state);
		}

		if (state) {
			if (id == Controller::MENU_BUTTON) {
				OptionListPage::set_text(list_option_text);
				OptionListPage::set_callback(select_edit_option);
				OptionListPage::set_count(NUM_LIST_OPTIONS);
				pages_->open(Pages::OPTION_LIST_PAGE);
				return;
			}

			if (shift) {
				if (id == Controller::UP_BUTTON) {
					zoom_ = SettingsUtils::clip(1, 8, zoom_ - 1);
					return;
				}

				if (id == Controller::DOWN_BUTTON) {
					zoom_ = SettingsUtils::clip(1, 8, zoom_ + 1);
					return;
				}
			}
		}
	}

	void refresh_leds() {
		ListPage::refresh_leds();
	}

	void draw() {
		ListPage::draw();

		const int x = 2;
		const int y = 2;
		const int w = 251;
		const int h = 47;
		const int center_h = h / 2;

		Sample *sample = settings_->selected_sample();
		size_t size = sample->size_samples() / zoom_;
		size_t offset = ((sample->size_samples() - size) / w) * sample_x_;
		size_t inc = (size / w);

		int16_t left = 0;
		int16_t right = 0;
		size_t index = offset;

		for (int x2 = 0; x2 < w; ++x2) {
			sample->read(index, &left, &right);
			index += inc;

			float left_ = (left + 32768) * (1.f / 65535.f);
			int y_left = center_h * (1.f - left_);
			int h_left = center_h - y_left;
			canvas_->vertical_line(x + x2, y + y_left, h_left, Canvas::GRAY);

			float right_ = (right + 32768) * (1.f / 65535.f);
			int y_right = center_h;
			int h_right = center_h * right_;
			canvas_->vertical_line(x + x2, y + y_right, h_right, Canvas::GRAY);
		}

		//if (sample->start() >= offset && sample->start() < (offset + size)) {
		//	float start = (sample->start() / float(size)) * w;
		//	canvas_->vertical_line((x + start) - sample_x_, y, h, Canvas::BLACK);
		//}

		float start = (sample->start() / float(size)) * w;
		canvas_->vertical_line(x + start, y, h, Canvas::BLACK);

		float end = (sample->end() / float(size)) * w;
		canvas_->vertical_line(x + end, y, h, Canvas::BLACK);

		float loop_start = (sample->loop_start() / float(size)) * w;
		canvas_->vertical_line(x + loop_start, y, h, Canvas::BLACK);

		float loop_end = (sample->loop_end() / float(size)) * w;
		canvas_->vertical_line(x + loop_end, y, h, Canvas::BLACK);

		int count = engine_->voiceEngine().num_active();
		for (int i = 0; i < count; ++i) {
			Voice &voice = engine_->voiceEngine().young_to_old(i);
			if (&voice.sample() == sample) {
				float play_position = (voice.phase() / float(size)) * w;
				canvas_->vertical_line(x + play_position, y, h, Canvas::BLACK);
				break;
			}
		}

		canvas_->draw_text(x, y, w, h, str_.write("X", zoom_), Canvas::RIGHT, Canvas::BOTTOM);
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
