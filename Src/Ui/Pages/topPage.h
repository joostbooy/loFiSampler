#ifndef TopPage_h
#define TopPage_h

#include "page.h"

class TopPage : public Page {

public:

	void init() override {
		Controller::init();
		//	MessagePainter::init(ui_);
		//	TextBufferPainter::init(ui_);
		//	WindowPainter::init(ui_);
	}

	void enter() override {

	}

	void exit() override {
		pageManager_->pages(PageManager::LIST_PAGE)->exit();
	}

	void on_button(int id, int state) override {
		Controller::on_button(id, state);
		//ChapterPage::onButton(id, state);
	}

	void on_encoder(int id, int state) override {
		Controller::on_encoder(id);
	}

	void draw_leds() override {

	}

	void draw_display() override {
		//MessagePainter::tick(ui_->pages().targetFps());
		MessagePainter::draw();
	}

	const size_t target_fps() override {
		return 1000 / 16;
	}

private:


};

#endif
