#include "pages.h"
#include "topPage.h"
#include "envelopePage.h"
#include "listPage.h"
#include "confirmationPage.h"


Pages::Page* page_[Pages::NUM_PAGES] = {
	[Pages::TOP_PAGE]			= &TopPage::page,
	[Pages::LIST_PAGE]			= &ListPage::page,
	[Pages::ENVELOPE_PAGE]		= &EnvelopePage::page,
	[Pages::CONFIRMATION_PAGE]	= &ConfirmationPage::page
};

void Pages::init(Settings *settings, Engine *engine, Canvas *canvas) {
	curr_page_ = TOP_PAGE;
	page_stack_.clear();
	TopPage::init(settings, engine, canvas, this);
	for (int i = 0; i < NUM_PAGES; ++i) {
		page_[i]->init();
	}
}

void Pages::open(PageId id) {
	if (page_stack_.find(id) == false && page_stack_.writeable() == true) {
		page_stack_.push(id);
		page_[id]->enter();
		curr_page_ = id;
	}
}

void Pages::close(PageId id) {
	if (page_stack_.remove_by_value(id)) {
		page_[id]->exit();
		if (page_stack_.readable()) {
			curr_page_ = page_stack_.read(page_stack_.size() - 1);
		} else {
			curr_page_ = TOP_PAGE;
		}
	}
}

void Pages::close_all() {
	while (page_stack_.readable()) {
		int id = page_stack_.pull();
		page_[id]->exit();
	}
	curr_page_ = TOP_PAGE;
}

void Pages::on_button(int id, int state) {
	page_[curr_page_]->on_button(id, state);
	page_[TOP_PAGE]->on_button(id, state);
}

void Pages::on_encoder(int id, int state) {
	page_[curr_page_]->on_encoder(id, state);
	page_[TOP_PAGE]->on_encoder(id, state);
}

void Pages::draw_leds() {
	for (int i = 0; i < page_stack_.size(); ++i) {
		int id = page_stack_.read(i);
		page_[id]->draw_leds();
	}
	page_[TOP_PAGE]->draw_leds();
}

void Pages::draw_display() {
	for (int i = 0; i < page_stack_.size(); ++i) {
		int id = page_stack_.read(i);
		page_[id]->draw_display();
	}
	page_[TOP_PAGE]->draw_display();
}

const size_t Pages::target_fps() {
	return page_[curr_page_]->target_fps();
}
