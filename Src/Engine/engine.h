#ifndef Engine_h
#define Engine_h

#include "uart.h"
#include "gate.h"
#include "micros.h"
#include "midiEngine.h"
#include "midiClockEngine.h"
#include "envelopeEngine.h"
#include "modulationEngine.h"
#include "voiceEngine.h"
#include "sampleQue.h"

class Engine {

public:

	enum Request {
		STOP				= (1 << 0),
		KILL_MIDI_CHANNEL	= (1 << 1),
	};

	void init(Settings*, Uart*, Usb*, Gate*);
	void tick();
	void suspend();
	void resume();
	void fill(Dac::Buffer *buffer, const size_t size);

	bool last_gate(int index) {
		return last_gate_[index];
	}

	VoiceEngine &voiceEngine() {
		return voiceEngine_;
	}

	ModulationEngine &modulationEngine() {
		return modulationEngine_;
	}

	void set_midi_channel_to_kill(uint8_t port, uint8_t channel) {
		port_to_kill_ = port;
		channel_to_kill_ = channel;
	}

	void add_request_blocking(Request type) {
		add_request(type);
		while (requests_ & type);
	}

private:
	volatile uint8_t requests_ = 0x00;

	uint8_t port_to_kill_;
	uint8_t channel_to_kill_;

	Gate *gate_;
	Settings *settings_;
	SampleQue sampleQue_;
	MidiEngine midiEngine_;
	VoiceEngine voiceEngine_;
	MidiClockEngine midiClockEngine_;
	ModulationEngine modulationEngine_;
	bool last_gate_[Modulation::kNumGatesToNote];

	void start();
	void stop();
	void process_requests();
	void process_midi();
	void process_gates();
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

	void clear_request(Request type) {
		uint8_t flags = requests_;
		requests_ = flags & ~type;
	}
};

#endif
