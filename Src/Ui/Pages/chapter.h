#ifndef Chapter_h
#define Chapter_h

#define TABLE_SIZE(table) (sizeof(table) / sizeof(int))

#include "pages.h"

class Chapter {

public:

	class Entry {

	public:

		void init(const int *page_table, const char* const *page_names, int size) {
			page_table_ = page_table;
			page_names_ = page_names;
			size_ = size;
		}

		int size() {
			return size_;
		}

		int page_index() {
			return pages_index_;
		}

		void set_page_index(int index) {
			pages_index_ = SettingsUtils::clip(0, size_ - 1, index);
		}

		int selected_page() {
			return page_table_[pages_index_];
		}

		const char *name(int index) {
			return page_names_[index];
		}

	private:
		int pages_index_ = 0;
		int size_;
		const int *page_table_;
		const char* const* page_names_;
	};

	enum Id {
		MIDI,
		MODULATION,
		SYSTEM,
		STORAGE,
		INSTRUMENT,
		SAMPLE,

		NUM_CHAPTERS
	};

	const char *name() {
		switch (selected_)
		{
		case MIDI:			return "MIDI";
		case MODULATION:	return "MODULATION";
		case SYSTEM:		return "SYSTEM";
		case STORAGE:		return "STORAGE";
		case INSTRUMENT:	return "INSTRUMENT";
		case SAMPLE:		return "SAMPLE";
		default:
			break;
		}
		return nullptr;
	}

	void init() {
		modulation_.init(modulation_pages, mod_page_names, TABLE_SIZE(modulation_pages));
		midi_.init(midi_pages, midi_page_names, TABLE_SIZE(midi_pages));
		sample_.init(sample_pages, sample_page_names, TABLE_SIZE(sample_pages));
		system_.init(system_pages, system_page_names, TABLE_SIZE(system_pages));
		storage_.init(storage_pages, storage_page_names, TABLE_SIZE(storage_pages));
		instrument_.init(instrument_pages, instrument_page_names, TABLE_SIZE(instrument_pages));
	}

	int selected() {
		return selected_;
	}

	void set(int index) {
		selected_ = SettingsUtils::clip(0, NUM_CHAPTERS - 1, index);

		switch (selected_)
		{
		case MIDI:
			entry_ = &midi_;
			break;
		case MODULATION:
			entry_ = &modulation_;
			break;
		case SAMPLE:
			entry_ = &sample_;
			break;
		case INSTRUMENT:
			entry_ = &instrument_;
			break;
		case STORAGE:
			entry_ = &storage_;
			break;
		case SYSTEM:
			entry_ = &system_;
			break;
		default:
			break;
		}
	}

	Entry *entry() {
		return entry_;
	}

private:
	Entry *entry_;
	int selected_ = SAMPLE;

	// Midi
	Entry midi_;

	static constexpr int midi_pages[] = {
		Pages::MIDI_PAGE,
		Pages::HARDWARE_TEST_PAGE
	};

	static constexpr const char* const midi_page_names[] = {
		"SETTINGS",
		"TEST"
	};

	// Modulation
	Entry modulation_;

	static constexpr int modulation_pages[] = {
		Pages::MODULATION_PAGE,
		Pages::LFO_PAGE,
		Pages::ENVELOPE_PAGE,
		Pages::DELAY_PAGE,
	};

	static constexpr const char* const mod_page_names[] = {
		"SETTINGS",
		"LFO",
		"ENVELOPE",
		"DELAY"
	};

	// System
	Entry system_;

	static constexpr int system_pages[] = {
		Pages::MODULATION_PAGE,
		Pages::HARDWARE_TEST_PAGE
	};

	static constexpr const char* const system_page_names[] = {
		"SETTINGS",
		"TEST"
	};

	// Instrument
	Entry instrument_;

	static constexpr int instrument_pages[] = {
		Pages::INSTRUMENT_PAGE
	};

	static constexpr const char* const instrument_page_names[] = {
		"SETTINGS",
	};

	// Sample
	Entry sample_;

	static constexpr int sample_pages[] = {
		Pages::SAMPLE_PAGE,
	};

	static constexpr const char* const sample_page_names[] = {
		"SETTINGS",
	};

	// Storage
	Entry storage_;

	static constexpr int storage_pages[] = {
		Pages::MODULATION_PAGE,
		Pages::HARDWARE_TEST_PAGE
	};

	static constexpr const char* const storage_page_names[] = {
		"SETTINGS",
		"TEST"
	};
};

#endif
