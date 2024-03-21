#ifndef MidiEngine_h
#define MidiEngine_h

#include "usb.h"
#include "uart.h"
#include "que.h"


class MidiEngine {

public:

	enum Port {
		UART,
		USB,
		// CV
		NUM_PORTS
	};

	enum MessageType {
		NOTE_OFF			= 0x80,
		NOTE_ON				= 0x90,
		KEY_PRESSURE		= 0xA0,
		CONTROLLER_CHANGE	= 0xB0,
		PROGRAM_CHANGE		= 0xC0,
		CHANNEL_PRESSURE	= 0xD0,
		PITCH_BEND			= 0xE0,
		CLOCK_PULSE			= 0xF8,
		CLOCK_START			= 0xFA,
		CLOCK_CONTINUE		= 0xFB,
		CLOCK_STOP			= 0xFC,
		SONG_POSITION		= 0xF2,
	};

	struct Event {
		uint8_t port;
		uint8_t message;
		uint8_t data[2];
	};

	void init(Uart *uart, Usb *usb) {
		usb_ = usb;
		uart_ = uart;
		num_data_bytes_ = 0;
	}

	void poll() {
		if (uart_->readable() && input_que.writeable()) {
			input_que.write(uart_->read());
		}

		if (uart_->writeable()) {
			if (clock_output_que.readable()) {
				uart_->write(clock_output_que.read());
			} else if (output_que.readable()) {
				uart_->write(output_que.read());
			}
		}
	}

	bool pull(Event &e) {
		while (input_que.readable()) {
			if (parse(input_que.read())) {
				e = event_;
				return true;
			}
		}
		return false;
	}

	static inline uint16_t read_14_bit(Event &e) {
		return (e.data[0] & 0x7F) | (e.data[1] << 7);
	}

	bool write(Event &e) {
		uint8_t size = e.message != last_message_ ? 3 : 2;
		if (output_que.available_size() >= size) {
			if (size == 3) {
				output_que.write(e.message);
				last_message_ = e.message;
			}
			output_que.write(e.data[0]);
			output_que.write(e.data[1]);
			return true;
		}
		return false;
	}

	bool write(uint8_t message) {
		if (clock_output_que.writeable()) {
			clock_output_que.write(message);
			return true;
		}
		return false;
	}

private:
	Usb *usb_;
	Uart *uart_;
	Event event_;
	uint8_t last_message_;
	uint8_t num_data_bytes_;
	Que<uint8_t, 32> input_que;
	Que<uint8_t, 32> output_que;
	Que<uint8_t, 8> clock_output_que;

	bool parse(uint8_t reading) {
		if (reading >= 0x80) {
			event_.message = reading;
			num_data_bytes_ = 0;
		} else {
			event_.data[num_data_bytes_] = reading;
			++num_data_bytes_;

			if (num_data_bytes_ >= 2) {
				num_data_bytes_ = 0;
				return true;
			}
		}
		return false;
	}
};

#endif
