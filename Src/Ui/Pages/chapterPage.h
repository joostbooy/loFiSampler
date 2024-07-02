#ifndef ChapterPage_h
#define ChapterPage_h

namespace ChapterPage {
	//Declarations
	void init();
	void enter();
	void exit();
	void msTick(uint16_t ticks);
	void drawDisplay();
	void updateLeds();
	void onButton(uint8_t id, uint8_t value);
	void onEncoder(uint8_t id, int inc);
	const uint16_t targetFps();

	// Variables
	enum Chapters {
		SAMPLE,
		INSTRUMENT,
		MODULATION,
		SYSTEM,

		NUM_CHAPTERS
	};

	int selected_chapter;
	int selected_page[NUM_CHAPTERS];

	void set_chapter(int id) {
		// pages.close_all();
		// pages.open(selected_page[chapter]);
		// pages.open_on_top(Pages::CHAPTER_PAGE);
	}

	int button_to_chapter(int id) {
		switch (id)
		{
		case Controller::SAMPLE_CHAPTER_BUTTON:		return ChapterPage::SAMPLE;
		case Controller::INSTRUMENT_CHAPTER_BUTTON:	return ChapterPage::INSTRUMENT;
		case Controller::MODULATION_CHAPTER_BUTTON:	return ChapterPage::MODULATION;
		case Controller::SYSTEM_CHAPTER_BUTTON:		return ChapterPage::SYSTEM;
		default:
			break;
		}
		return -1;
	}

	void init() {
		selected_chapter = SAMPLE;
	}

	void enter() {

	}

	void exit() {

	}

	void onEncoder(uint8_t id, int inc) {
		// selected_chapter = stmlib::clip(0, NUM_CHAPTERS - 1, selected_chapter + inc);
	}

	void onButton(uint8_t id, uint8_t state) {
		int chapter = button_to_chapter(id);

		if (chapter >= 0) {
			if (state > 0) {
				set_chapter(chapter);
				//pages.open(Pages::CHAPTER_PAGE);
			} else {
				//pages.close(Pages::CHAPTER_PAGE);
			}
		}
	}

	void drawLeds() {

	}

	void msTick(uint16_t ticks) {

	}

	// Bottom to top
	void drawDisplay() {

	}

	const uint16_t targetFps() {
		return 1000 / 16;
	}

	const Pages::EventHandlers eventHandlers = {
		&init,
		&enter,
		&exit,
		&msTick,
		&drawLeds,
		&drawDisplay,
		&onEncoder,
		&onButton,
		&targetFps,
	};


};

#endif
