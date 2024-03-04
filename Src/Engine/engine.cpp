#include "engine.h"

Engine engine;

void Engine::init(Uart *uart) {
	midiEngine_.init(&uart);

	for (int i = 0; i < MidiEngine::NUM_PORTS; ++i)	{
		pitch_bend_value_[i] = 0.f
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
	midiEngine_.tick();
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
		switch (e.message)
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

float Engine::processing_time() {
	return (1.f / 500.f) * processing_time_;
}

// 2 khz interrupt
void Engine::update() {
	uint32_t start = micros.read();

	//

	processing_time_ = micros.read() - start;
}
