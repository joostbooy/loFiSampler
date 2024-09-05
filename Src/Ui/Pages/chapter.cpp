#include "chapter.h"

// Midi
 constexpr const int Chapter::midi_pages[] = {
	Pages::MIDI_PAGE,
};

 constexpr const char* const Chapter::midi_page_names[] = {
	"SETTINGS",
};

// Modulation
constexpr const int Chapter::modulation_pages[] = {
	Pages::MODULATION_PAGE,
	Pages::LFO_PAGE,
	Pages::ENVELOPE_PAGE,
	Pages::DELAY_PAGE,
};

constexpr const char* const Chapter::modulation_page_names[] = {
	"SETTINGS",
	"LFO",
	"ENVELOPE",
	"DELAY"
};

// System
constexpr const int Chapter::system_pages[] = {
	Pages::HARDWARE_TEST_PAGE
	//Pages::SYSTEM_INFO_PAGE
};

constexpr const char* const Chapter::system_page_names[] = {
	"HARDWARE TEST"
	//"INFO"
};

// Instrument
constexpr const int Chapter::instrument_pages[] = {
	Pages::INSTRUMENT_PAGE
};

constexpr const char* const Chapter::instrument_page_names[] = {
	"SETTINGS",
};

// Sample
constexpr const int Chapter::sample_pages[] = {
	Pages::SAMPLE_PAGE,
};

constexpr const char* const Chapter::sample_page_names[] = {
	"SETTINGS",
};

// Storage
constexpr const int Chapter::storage_pages[] = {
	Pages::PROJECT_PAGE
};

constexpr const char* const Chapter::storage_page_names[] = {
	"PROJECT STORAGE"
};

void Chapter::init() {
	modulation_.init(modulation_pages, modulation_page_names, ENTRY_SIZE(modulation_pages));
	midi_.init(midi_pages, midi_page_names, ENTRY_SIZE(midi_pages));
	sample_.init(sample_pages, sample_page_names, ENTRY_SIZE(sample_pages));
	system_.init(system_pages, system_page_names, ENTRY_SIZE(system_pages));
	storage_.init(storage_pages, storage_page_names, ENTRY_SIZE(storage_pages));
	instrument_.init(instrument_pages, instrument_page_names, ENTRY_SIZE(instrument_pages));
}
