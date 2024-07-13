#ifndef Debug_h
#define Debug_h

#include "stm32f7xx.h"

class Debug {

public:
    static void init();

	static inline void write(bool state) {
        GPIOG->BSRR = state ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    }

    inline void toggle() {
		(toggle_state ^= 1) ? write(1) : write(0);
    }

private:
	static uint8_t toggle_state;
};

#endif
