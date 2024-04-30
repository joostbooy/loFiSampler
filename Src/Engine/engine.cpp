#include "engine.h"

Engine engine;

void Engine::init(Uart *uart, Usb* usb) {
	//midiEngine_.init(uart, usb);
	modualationEngine_.init(&settings);
	voiceEngine_.init();

	for (size_t i = 0; i < Modulation::kNumGatesToNote; ++i) {
		last_gate_[i] = 0;
	}
}

/*	Engine commands */

void Engine::start() {

}

void Engine::stop() {

}

void Engine::suspend() {

}

void Engine::resume() {

}

void Engine::tick() {
	if (midiClockEngine_.tick()) {
		for (int i = 0; i < MidiEngine::NUM_PORTS; ++i) {
			if (settings.midi().send_clock(i)) {
				midiEngine_.write(i, MidiEngine::CLOCK_PULSE);
			}
		}
	}
	midiEngine_.poll();
}

void Engine::note_on(MidiEngine::Event &e) {
	size_t num_written = sampleQue_.note_on(e);
	if (num_written >= 1) {
		voiceEngine_.request_voices(num_written);
	}
}

void Engine::note_off(MidiEngine::Event &e) {
	voiceEngine_.note_off(e.port, e.message & 0x0F, e.data[0]);
}

void Engine::pitch_bend(MidiEngine::Event &e) {
	float data = (1.f / 16383.f) * MidiEngine::read_14_bit(e);
	modualationEngine_.write_midi_bend(data);
}

void Engine::cc(MidiEngine::Event &e) {
	uint8_t number = e.data[0];
	float data = (1.f / 127.f) * e.data[1];
	modualationEngine_.write_midi_cc(number, data);
}

void Engine::process_gates() {
	MidiEngine::Event e;

	for (size_t i = 0; i < Modulation::kNumGatesToNote; ++i) {
		bool current = gate.read(i);

		if (current != last_gate_[i]) {
			last_gate_[i] = current;
			e = settings.modulation().gate_to_midi(i);
			current ? note_on(e) : note_off(e);
		}
	}
}

//void Engine::clock_start() {
//	modualationEngine_.reset();
//}

// low priority
void Engine::process_midi() {
	MidiEngine::Event e;

	while (midiEngine_.pull(e)) {
		switch (e.message & 0xF0)
		{
		case MidiEngine::NOTE_ON:
			note_on(e);
			break;
		case MidiEngine::NOTE_OFF:
			note_off(e);
			break;
		case MidiEngine::PITCH_BEND:
			pitch_bend(e);
			break;
		case MidiEngine::CONTROLLER_CHANGE:
			cc(e);
			break;
		default:
			break;
		}
	}
}

void Engine::process_requests() {
	if (!requests_) {
		return;
	}

	if (requests_ & STOP) {
		// just keeps clearing the que & voices till idle
		sampleQue_.init();
		for (size_t i = 0; i < Settings::kMaxVoices; ++i) {
			voiceEngine_.voice(i).request_stop();
		}

		if (voiceEngine_.is_idle()) {
			clear_request(STOP);
		}
	}

	if (requests_ & KILL_MIDI_PORT) {
		clear_request(KILL_MIDI_PORT);
		voiceEngine_.kill_midi_port(port_to_kill_);
	}
}

void Engine::fill(Dac::Buffer *buffer, const size_t size) {
	process_midi();
	process_gates();
	process_requests();

	bool new_voices = false;

	while (sampleQue_.readable() && voiceEngine_.available()) {
		voiceEngine_.assign_voice(sampleQue_.read());
		new_voices = true;
	}

	if (new_voices) {
		modualationEngine_.retrigger_lfos();
	}
	modualationEngine_.tick_lfos();

	voiceEngine_.fill(buffer, size);
	voiceEngine_.update_available_voices();
}
