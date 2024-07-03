#include "page.h"
#include "PageManager.h"
#include "confirmationPage.h"
#include "envelopePage.h"
#include "listPage.h"
#include "topPage.h"

TopPage topPage;
ListPage listPage;
EnvelopePage envelopePage;
ConfirmationPage confirmationPage;

Page* PageManager::pages_[PageManager::NUM_PAGES] = {
	[PageManager::TOP_PAGE]				= &topPage,
	[PageManager::LIST_PAGE]			= &listPage,
	[PageManager::ENVELOPE_PAGE]		= &envelopePage,
	[PageManager::CONFIRMATION_PAGE]	= &confirmationPage
};

void PageManager::init(Settings *settings, Engine *engine, Canvas *canvas) {
	Page::init(settings, engine, canvas, this);
	for (int i = 0; i < NUM_PAGES; ++i) {
		pages_[i]->init();
	}
	curr_page_ = TOP_PAGE;
	page_stack_.clear();
}

void PageManager::open(PageId id) {
	if (page_stack_.find(id) == false && page_stack_.writeable() == true) {
		page_stack_.push(id);
		pages_[id]->enter();
		curr_page_ = id;
	}
}

void PageManager::close(PageId id) {
	if (page_stack_.remove_by_value(id)) {
		pages_[id]->exit();
		if (page_stack_.readable()) {
			curr_page_ = page_stack_.read(page_stack_.size() - 1);
		}
	}
}

void PageManager::close_all() {
	while (page_stack_.readable()) {
		int id = page_stack_.pull();
		pages_[id]->exit();
	}
	curr_page_ = TOP_PAGE;
}

void PageManager::on_button(int id, int state) {
	pages_[curr_page_]->on_button(id, state);
	//pages_[TOP_PAGE]->on_button(id, state);
}

void PageManager::on_encoder(int id, int state) {
	pages_[curr_page_]->on_encoder(id, state);
	//pages_[TOP_PAGE]->on_encoder(id, state);
}

void PageManager::draw_leds() {
	for (int i = 0; i < page_stack_.size(); ++i) {
		pages_[i]->draw_leds();
	}
	//pages_[TOP_PAGE]->draw_leds();
}

void PageManager::draw_display() {
	for (int i = 0; i < page_stack_.size(); ++i) {
		pages_[i]->draw_display();
	}
	//	pages_[TOP_PAGE]->draw_display();
}

const size_t PageManager::target_fps() {
	return pages_[curr_page_]->target_fps();
}
