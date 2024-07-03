#ifndef Debug_h
#define Debug_h

#include "stm32f4xx.h"

class Debug {

public:
    static void init();

	static inline bool read() {
		return GPIOE->IDR & GPIO_PIN_0;
	}

	static inline void write(bool state) {
        GPIOE->BSRR = state ? GPIO_PIN_1 : GPIO_PIN_1 << 16;
    }

    inline void toggle() {
		(toggle_state ^= 1) ? write(1) : write(0);
    }

private:
	static uint8_t toggle_state;
};

#endif
