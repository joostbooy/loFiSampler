#include "engine.h"

Engine engine;

void Engine::init(Uart *uart, Usb* usb) {
	//midiEngine_.init(uart, usb);
	modualationEngine_.init(&settings);

	for (size_t i = 0; i < Settings::kNumEnvelopes; ++i) {
		for (size_t v = 0; v < Settings::kMaxVoices; ++v) {
			envelopeEngine_[v + (i * Settings::kMaxVoices)].init(&settings.envelope(i));
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

void Engine::fill(Dac::Buffer *buffer, const size_t size) {
	process_midi();

	SampleQue::Event e;
	while (sampleQue_.readable() && voiceEngine_.available()) {
		e = sampleQue_.read();
		voiceEngine_.assign_voice(e);
	}

	for (size_t i = 0; i < size; ++i) {
		//
	}

	voiceEngine_.update_available_voices();
}
