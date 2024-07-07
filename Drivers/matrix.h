#ifndef Matrix_h
#define Matrix_h

#include "stm32f4xx.h"
#include "micros.h"

// Button led matrix 8x16

class Matrix {

public:

	enum LedColor {
		BLACK	= 0x00,
		RED		= 0x01,
	};

	void init();

	void lock_leds() {
		for (int i = 0; i < kNumOfLedCollumns; ++i) {
			led_row_copy[i] = led_row[i];
		}
		leds_locked = true;
	}

	void unlock_leds() {
		leds_locked = false;
	}

	void clear_leds() {
		for (int i = 0; i < kNumOfLedCollumns; ++i) {
			led_row[i] = 0;
		}
	}

	void set_led(uint8_t coll, uint8_t row, LedColor color) {
		uint8_t data = led_row[coll] & ~(1 << row);
		led_row[coll] = data | (color << row);
	}

	void refresh(uint8_t *buffer) {
		for (int i = 0; i < kNumOfSwitchCollumns; ++i) {
			set_collumn(i);
			GPIOE->BSRR = GPIO_PIN_6 << 16;				// latch switches
			asm("NOP");
			GPIOE->BSRR = GPIO_PIN_6;
			*buffer++ = spi_transfer();
		}

		// write led row
		if (leds_locked) {
			spi_transfer(led_row_copy[led_coll]);
		} else {
			spi_transfer(led_row[led_coll]);
		}

		// update collumn
		set_collumn(led_coll);
		++led_coll;
		if (led_coll >= kNumOfLedCollumns) {
			led_coll = 0;
		}

		// enable led row &  collumns
		GPIOB->BSRR = GPIO_PIN_8 << 16;
		GPIOB->BSRR = GPIO_PIN_9 << 16;
		// latch led rows & collumn
		GPIOB->BSRR = GPIO_PIN_8 | GPIO_PIN_9;
	}

private:
	static const uint8_t kNumOfSwitchCollumns = 7;
	static const uint8_t kNumOfLedCollumns = 7;
	volatile bool leds_locked = false;

	uint8_t led_coll = 0;
	uint8_t led_row[kNumOfLedCollumns];
	uint8_t led_row_copy[kNumOfLedCollumns];

	void set_collumn(uint8_t coll) {
		uint32_t reg = 0;
		coll & 0x01 ? reg |= GPIO_PIN_2 : reg |= GPIO_PIN_2 << 16;
		coll & 0x02 ? reg |= GPIO_PIN_3 : reg |= GPIO_PIN_3 << 16;
		coll & 0x04 ? reg |= GPIO_PIN_4 : reg |= GPIO_PIN_4 << 16;
		GPIOE->BSRR = reg;
		Micros::delay(1);
	}

	uint8_t spi_transfer(uint8_t send = 0xff) {
		volatile uint8_t recv;

		while (!(SPI3->SR & SPI_FLAG_TXE));
		SPI3->DR = send;

		while (!(SPI3->SR & SPI_FLAG_RXNE));
		recv = SPI3->DR;

		return recv;
	}

};

#endif
