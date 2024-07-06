#include "pages.h"
#include "topPage.h"
#include "lfoPage.h"
#include "envelopePage.h"
#include "instrumentPage.h"
#include "listPage.h"
#include "confirmationPage.h"
#include "midiPage.h"
#include "modulationPage.h"
#include "samplePage.h"
#include "hardwareTestPage.h"

Pages::Page* page_[Pages::NUM_PAGES] = {
	[Pages::TOP_PAGE]			= &TopPage::page,
	[Pages::LIST_PAGE]			= &ListPage::page,
	[Pages::LFO_PAGE]			= &LfoPage::page,
	[Pages::ENVELOPE_PAGE]		= &EnvelopePage::page,
	[Pages::INSTRUMENT_PAGE]	= &InstrumentPage::page,
	[Pages::MIDI_PAGE]			= &MidiPage::page,
	[Pages::MODULATION_PAGE]	= &ModulationPage::page,
	[Pages::SAMPLE_PAGE]		= &SamplePage::page,
	[Pages::CONFIRMATION_PAGE]	= &ConfirmationPage::page,
	[Pages::HARDWARE_TEST_PAGE]	= &HardwareTestPage::page
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
		int id = page_stack_.pop();
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

void Pages::refresh_leds() {
	for (int i = 0; i < page_stack_.size(); ++i) {
		int id = page_stack_.read(i);
		page_[id]->refresh_leds();
	}
	page_[TOP_PAGE]->refresh_leds();
}

void Pages::draw() {
	for (int i = 0; i < page_stack_.size(); ++i) {
		int id = page_stack_.read(i);
		page_[id]->draw();
	}
	page_[TOP_PAGE]->draw();
}

const size_t Pages::target_fps() {
	return page_[curr_page_]->target_fps();
}
