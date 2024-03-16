#ifndef Engine_h
#define Engine_h

#include "micros.h"
#include "midiClockEngine.h"
#include "midiEngine.h"
#include "uart.h"
#include "lfoEngine.h"
#include "envelopeEngine.h"

class Engine {

public:

	enum State {
		STOPPED,
		PAUSED,
		RUNNING
	};

	enum Request {
		START			= (1 << 0),
		STOP			= (1 << 1),
		PAUSE			= (1 << 2),
		CONTINUE		= (1 << 3),
	};

	void init(Uart*, Usb*);
	void tick();
	void suspend();
	void resume();
	void process();
	void fill(Dac::Buffer *buffer, const size_t size);

	float processing_time();

	// states
	State state() {
		return state_;
	}

	constexpr size_t max_voices() {
		return kMaxVoices;
	}

	// requests
	void add_request_wait(Request type) {
		add_request(type);
		while (requests & type);
	}

	void clear_request(Request type) {
		uint8_t flags = requests;
		requests = flags & ~type;
	}

	MidiEngine &midiEngine() {
		return midiEngine_;
	}

private:
	volatile State state_;
	volatile uint8_t requests = 0x00;
	float pitch_bend_value_[MidiEngine::NUM_PORTS];

	static const size_t kMaxVoices = 8;

	MidiEngine midiEngine_;
	MidiClockEngine midiClockEngine_;
	LfoEngine lfoEngine_[Settings::num_lfos()];
	EnvelopeEngine envelopeEngine_[Settings::num_envelopes() * kMaxVoices];


	void start();
	void stop();
	void process_requests();
	void note_on(MidiEngine::Event &e);
	void note_off(MidiEngine::Event &e);
	void pitch_bend(MidiEngine::Event &e);

	void send_midi_clock_start();
	void send_midi_clock_stop();
	void send_midi_clock_continue();
	void send_midi_24PPQN_clock_tick();

	void add_request(Request type) {
		uint8_t flags = requests;
		requests = flags | type;
	}
};

extern Engine engine;

#endif