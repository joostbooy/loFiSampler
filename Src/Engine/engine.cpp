#include "engine.h"

Engine engine;

void Engine::init(Uart *uart, Usb* usb) {
	midiEngine_.init(uart, usb);
	modualationEngine_.init(&settings);

	for (size_t i = 0; i < Settings::num_envelopes(); ++i) {
		for (size_t v = 0; v < kMaxVoices; ++v) {
			envelopeEngine_[v + (i * kMaxVoices)].init(&settings.envelope(i));
		}
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

/*	Tasks */

void Engine::process_requests() {

}

void Engine::tick() {
	if (midiClockEngine_.tick()) {
		if (settings.midi().send_clock()) {
			midiEngine_.write(MidiEngine::CLOCK_PULSE);
		}
	}
	midiEngine_.poll();
}

void Engine::note_on(MidiEngine::Event &e) {
//	for (int i = 0; i < Settings::kNumInstruments; ++i) {
//		if (instrumentEngine_[i].)
//	}
}

void Engine::note_off(MidiEngine::Event &e) {

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

// low priority
void Engine::process() {
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

void Engine::fill(Dac::Buffer *buffer, const size_t size) {
	// process_midi();

	for (size_t i = 0; i < size; ++i) {
		//
	}
}
