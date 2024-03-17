#include "engine.h"

Engine engine;

void Engine::init(Uart *uart, Usb* usb) {
	midiEngine_.init(uart, usb);

	for (size_t i = 0; i < Settings::num_lfos(); ++i) {
		lfoEngine_[i].init(&settings.lfo(i));
	}

	for (size_t i = 0; i < Settings::num_envelopes(); ++i) {
		for (size_t v = 0; v < kMaxVoices; ++v) {
			envelopeEngine_[v + (i * kMaxVoices)].init(&settings.envelope(i));
		}
	}

	//modualationEngine_.init(&lfoEngine_[0], Settings::num_lfos(), &envelopeEngine_[0], )

	for (size_t i = 0; i < MidiEngine::NUM_PORTS; ++i)	{
		pitch_bend_value_[i] = 0.5f;
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

}

void Engine::note_off(MidiEngine::Event &e) {

}

void Engine::pitch_bend(MidiEngine::Event &e) {
	pitch_bend_value_[e.port] = (1.f / 16383.f) * MidiEngine::read_14_bit(e);
}

// low priority
void Engine::process() {

	MidiEngine::Event e;

	while (midiEngine_.read(e)) {
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
		default:
			break;
		}
	}
}

void Engine::fill(Dac::Buffer *buffer, const size_t size) {
	for (size_t i = 0; i < size; ++i) {
		//
	}
}
