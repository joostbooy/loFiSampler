#ifndef Engine_h
#define Engine_h

#include "uart.h"
#include "micros.h"
#include "midiEngine.h"
#include "midiClockEngine.h"
#include "envelopeEngine.h"
#include "modulationEngine.h"
#include "voiceEngine.h"
#include "sampleQue.h"

class Engine {

public:

	enum State {
		STOPPED,
		PAUSED,
		RUNNING
	};

	enum Request {
		START				= (1 << 0),
		STOP				= (1 << 1),
		PAUSE				= (1 << 2),
		CONTINUE			= (1 << 3),
		KILL_MIDI_CHANNEL	= (1 << 4),
	};

	void init(Uart*, Usb*);
	void tick();
	void suspend();
	void resume();
	void fill(Dac::Buffer *buffer, const size_t size);

	float processing_time();

	// states
	State state() {
		return state_;
	}

	// requests_
	void add_request_blocking(Request type) {
		add_request(type);
		while (requests_ & type);
	}

	void clear_request(Request type) {
		uint8_t flags = requests_;
		requests_ = flags & ~type;
	}

	MidiEngine &midiEngine() {
		return midiEngine_;
	}

	void kill_midi_channel_blocking(uint8_t channel) {
		channel_to_kill_ = channel;
		add_request_blocking(KILL_MIDI_CHANNEL);
	}

private:
	volatile State state_;
	volatile uint8_t requests_ = 0x00;

	uint8_t channel_to_kill_;

	SampleQue sampleQue_;
	MidiEngine midiEngine_;
	VoiceEngine voiceEngine_;
	MidiClockEngine midiClockEngine_;
	ModulationEngine modualationEngine_;
	EnvelopeEngine envelopeEngine_[Settings::kMaxInstruments * Settings::kMaxVoices];

	void start();
	void stop();
	void process_requests();
	void process_midi();
	void note_on(MidiEngine::Event &e);
	void note_off(MidiEngine::Event &e);
	void pitch_bend(MidiEngine::Event &e);
	void cc(MidiEngine::Event &e);

	void send_midi_clock_start();
	void send_midi_clock_stop();
	void send_midi_clock_continue();
	void send_midi_24PPQN_clock_tick();

	void add_request(Request type) {
		uint8_t flags = requests_;
		requests_ = flags | type;
	}
};

extern Engine engine;

#endif
