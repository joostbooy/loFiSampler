#ifndef VoiceEngine_h
#define VoiceEngine_h

#include "settings.h"
#include "voice.h"
#include "stack.h"
#include "sampleQue.h"

class VoiceEngine {

public:

	void init() {
		active_voices_.clear();
		available_voices_.clear();

		for (size_t i = 0; i < Settings::kMaxVoices; ++i) {
			voice_[i].init();
			available_voices_.push(i);
		}
	}

	inline uint8_t most_recent_voice() {
		return most_recent_voice_;
	}

	inline bool is_idle() {
		return active_voices_.size() == 0;
	}

	Voice &voice(uint8_t index) {
		return voice_[index];
	}

	void fill(Dac::Buffer *buffer, const size_t size) {
		std::fill(&buffer[0].channel[0], &buffer[size].channel[0], 0);

		for (size_t i = 0; i < Settings::kMaxVoices; ++i) {
			if (voice_[i].state() != Voice::IDLE) {
				voice_[i].fill(buffer, size);
			}
		}

		for (size_t i = 0; i < Dac::kNumChannels; ++i) {
			limiter_[i].process(&buffer[0].channel[i], Dac::kBlockSize, Dac::kNumChannels);
		}
	}

	void request_voices(size_t count) {
		if (count >= Settings::kMaxVoices) {
			count = Settings::kMaxVoices - 1;
		}

		while (available_voices_.size() < count) {
			uint8_t v = active_voices_.pull();
			voice_[v].request_stop();
			active_voices_.push(v);
			--count;
		}
	}

	void note_off(uint8_t port, uint8_t chn, uint8_t note) {
		uint8_t count = active_voices_.size();

		for (uint8_t i = 0; i < count; ++i) {
			uint8_t v = active_voices_.read(i);
			if (voice_[v].key_pressed() == true && voice_[v].port() == port && voice_[v].channel() == chn && voice_[v].note() == note) {
				voice_[v].note_off();
				break;
			}
		}
	}

	bool available() {
		return available_voices_.size() > 0;
	}

	void assign_voice(SampleQue::Event e) {
		uint8_t v = available_voices_.pop();
		voice_[v].note_on(e);
		active_voices_.push(v);
		most_recent_voice_ = v;
	}

	void update_available_voices() {
		uint8_t index = 0;
		while (index < active_voices_.size()) {
			uint8_t v = active_voices_.read(index);
			if (voice_[v].is_available()) {
				active_voices_.remove_indexed(index);
				available_voices_.push(v);
			} else {
				++index;
			}
		}
	}

	void kill_midi_port(uint8_t port) {
		uint8_t count = active_voices_.size();

		for (uint8_t i = 0; i < count; ++i) {
			uint8_t v = active_voices_.read(i);
			if (voice_[v].port() == port) {
				voice_[v].request_stop();
			}
		}
	}

private:
	Voice voice_[Settings::kMaxVoices];
	Stack<uint8_t, Settings::kMaxVoices> active_voices_;
	Stack<uint8_t, Settings::kMaxVoices> available_voices_;
	Limiter<int16_t> limiter_[Dac::kNumChannels];
	size_t most_recent_voice_ = 0;
};

#endif
