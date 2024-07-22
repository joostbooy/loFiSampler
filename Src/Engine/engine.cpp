#include "engine.h"

void Engine::init(Settings *settings, Uart *uart, Usb *usb, Gate *gate) {
	settings_ = settings;
	gate_ = gate;

	limiter_.init();
	midiEngine_.init(uart, usb);
	modulationEngine_.init(settings);
	voiceEngine_.init(settings, &modulationEngine_);
	sampleQue_.init(settings);
	delayEngine_.init(settings);

	for (size_t i = 0; i < Modulation::kNumGatesToNote; ++i) {
		last_gate_[i] = gate_->read(i);
	}
}

void Engine::start() {
	modulationEngine_.reset_lfos();

	for (int i = 0; i < Midi::NUM_PORTS; ++i) {
		if (settings_->midi().send_clock(i)) {
			midiEngine_.write_clock(i, MidiEngine::CLOCK_START);
		}
	}
}

void Engine::stop() {
	for (int i = 0; i < Midi::NUM_PORTS; ++i) {
		if (settings_->midi().send_clock(i)) {
			midiEngine_.write_clock(i, MidiEngine::CLOCK_STOP);
		}
	}
}

void Engine::tick() {
	if (midiClockEngine_.tick()) {
		for (int i = 0; i < Midi::NUM_PORTS; ++i) {
			if (settings_->midi().send_clock(i)) {
				midiEngine_.write_clock(i, MidiEngine::CLOCK_PULSE);
			}
		}
	}
	midiEngine_.poll();

	//	for (size_t i = 0; i < Modulation::kNumGatesToNote; ++i) {
	//		bool current = gate_->read(i);

	//		if (current != last_gate_[i]) {
	//			last_gate_[i] = current;
	//			e = settings_->modulation().gate_to_midi(i);
	//			current ? e.message |= MidiEngine::NOTE_ON : e.message |= MidiEngine::NOTE_OFF;
	//			midiEngine_.write_input(e);
	//		}
	//	}
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
	modulationEngine_.set_midi_bend(data);
}

void Engine::cc(MidiEngine::Event &e) {
	uint8_t number = e.data[0];
	float data = (1.f / 127.f) * e.data[1];
	modulationEngine_.set_midi_cc(number, data);
}

void Engine::process_gates() {
	MidiEngine::Event e;

	for (size_t i = 0; i < Modulation::kNumGatesToNote; ++i) {
		bool current = gate_->read(i);

		if (current != last_gate_[i]) {
			last_gate_[i] = current;
			e = settings_->modulation().gate_to_midi(i);
			current ? note_on(e) : note_off(e);
		}
	}
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

void Engine::process_requests() {
	if (!requests_) {
		return;
	}

	if (requests_ & STOP) {
		// just keeps clearing the que & voices till idle
		sampleQue_.clear();
		for (size_t i = 0; i < Settings::kMaxVoices; ++i) {
			voiceEngine_.voice(i).request_stop();
		}

		if (voiceEngine_.is_idle()) {
			clear_request(STOP);
		}
	}

	if (requests_ & KILL_MIDI_CHANNEL) {
		clear_request(KILL_MIDI_CHANNEL);
		voiceEngine_.kill_midi_channel(port_to_kill_, channel_to_kill_);
	}
}

void Engine::fill(Dac::Channel *channel, const size_t size) {
	process_midi();
	process_gates();
	process_requests();

	while (sampleQue_.readable() && voiceEngine_.available()) {
		SampleQue::Event e = sampleQue_.read();
		voiceEngine_.assign_voice(e);
		modulationEngine_.retrigger_lfos(e.midi_event_);
	}
	modulationEngine_.tick_lfos();

	std::fill(&channel[0].left[0], &channel[Dac::kNumStereoChannels].left[0], 0);
	
	voiceEngine_.fill(channel, size);
	delayEngine_.fill(channel, size);
	limiter_.fill(channel, size);
}
