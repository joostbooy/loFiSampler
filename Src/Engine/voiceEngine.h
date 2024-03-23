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

		for (uint8_t i = 0; i < Settings::kMaxVoices; ++i) {
		//	voice_[i].init(settings.envelope(0));
			available_voices_.push(i);
			key_is_pressed_[i] = false;
		}
	}

	inline uint8_t most_recent_voice() {
		return most_recent_voice_;
	}

	Voice &voice(uint8_t index) {
		return voice_[index];
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
		uint8_t total = active_voices_.size();

		for (int i = 0; i < total; ++i) {
			uint8_t v = active_voices_.read(i);
			if (key_is_pressed_[v] == true && voice_[v].port() == port && voice_[v].channel() == chn && voice_[v].note() == note) {
				key_is_pressed_[v] = false;
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
		voice_[v].note_on(e.midi_event_);
		active_voices_.push(v);

		most_recent_voice_ = v;
		key_is_pressed_[v] = true;
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

	void kill_midi_channel(uint8_t chn) {
		uint8_t total = active_voices_.size();

		for (int i = 0; i < total; ++i) {
			uint8_t v = active_voices_.read(i);
			if (voice_[v].channel() == chn) {
				voice_[v].request_stop();
				key_is_pressed_[v] = false;
			}
		}
	}

private:
	Voice voice_[Settings::kMaxVoices];
	Stack<uint8_t, Settings::kMaxVoices> active_voices_;
	Stack<uint8_t, Settings::kMaxVoices> available_voices_;

	bool key_is_pressed_[Settings::kMaxVoices];
	uint8_t most_recent_voice_ = 0;
};

#endif
