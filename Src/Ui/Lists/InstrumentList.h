#ifndef InstrumentList_h
#define InstrumentList_h

#include "settings.h"
#include "settingsText.h"

class InstrumentList : public SettingsList {

public:

	enum Item {
		INDEX,
		PAN,
		GAIN,
		BEND,
		AUDIO_CHANNEL,
		MIDI_PORT,
		MIDI_CHANNEL,
		BIT_DEPTH,
		SAMPLERATE,
		BEND_RANGE,

		NUM_ITEMS
	};

	const int num_items() override {
		return NUM_ITEMS;
	}

	const char* item_text(int item) override {
		switch (item)
		{
		case INDEX:				return "INSTRUMENT";
		case PAN:				return "PAN";
		case GAIN:				return "GAIN";
		case BEND:				return "BEND";
		case AUDIO_CHANNEL:		return "AUDIO CHANNEL";
		case MIDI_PORT:			return "MIDI PORT";
		case MIDI_CHANNEL:		return "MIDI CHANNEL";
		case BIT_DEPTH:			return "BIT DEPTH";
		case SAMPLERATE:		return "SAMPLE RATE";
		case BEND_RANGE:		return "BEND RANGE";
		default:
			break;
		}
		return nullptr;
	}

	const char* value_text(int item) override {
		Instrument &instrument = settings.selected_instrument();

		switch (item)
		{
		case INDEX:			return SettingsText::int_to_text(settings.selected_instrument_index() + 1);
		case PAN:			return instrument.pan_text();
		case GAIN:			return instrument.gain_text();
		case BEND:			return instrument.bend_text();
		case AUDIO_CHANNEL:	return instrument.audio_channel_text();
		case MIDI_PORT:		return instrument.midi_port_text();
		case MIDI_CHANNEL:	return instrument.midi_channel_text();
		case BIT_DEPTH:		return instrument.bit_depth_text();
		case SAMPLERATE:	return instrument.sample_rate_text();
		case BEND_RANGE:	return instrument.bend_range_text();
		default:
			break;
		}
		return nullptr;
	}

	void edit(int item, int inc, bool shifted) override {
		Instrument &instrument = settings.selected_instrument();

		switch (item)
		{
		case INDEX:
			settings.select_instrument_index(settings.selected_instrument_index() + inc);
			break;
		case PAN:
			instrument.set_pan(instrument.pan() + f_inc(inc, shifted));
			break;
		case GAIN:
			instrument.set_gain(instrument.gain() + f_inc(inc, shifted));
			break;
		case BEND:
			instrument.set_bend(instrument.bend() + f_inc(inc, shifted));
			break;
		case AUDIO_CHANNEL:
			instrument.set_audio_channel(instrument.audio_channel() + inc);
			break;
		case MIDI_PORT:
			kill_midi(instrument);
			instrument.set_midi_port(instrument.midi_port() + inc);
			break;
		case MIDI_CHANNEL:
			kill_midi(instrument);
			instrument.set_midi_channel(instrument.midi_channel() + inc);
			break;
		case BIT_DEPTH:
			instrument.set_bit_depth(instrument.bit_depth() + inc);
			break;
		case SAMPLERATE:
			instrument.set_sample_rate(instrument.sample_rate() + inc);
			break;
		case BEND_RANGE:
			instrument.set_bend_range(instrument.bend_range() + inc);
			break;
		default:
			break;
		}
	}

private:

	void kill_midi(Instrument &instrument) {
		engine.set_midi_channel_to_kill(instrument.midi_port(), instrument.midi_channel());
		engine.add_request_blocking(Engine::KILL_MIDI_CHANNEL);
	}

	inline float f_inc(int inc, bool shift) {
		return inc * (1.f / 100.f) * (shift ? 10.f : 1.f);
	}
};

#endif
