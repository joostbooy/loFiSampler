#ifndef Gate_h
#define Gate_h

#include "stm32f4xx.h"

class Gate {

public:
	void init();

	bool read(uint8_t channel) {
		switch (channel)
		{
		case 0: return !(GPIOB->IDR & GPIO_PIN_14);
		case 1: return !(GPIOB->IDR & GPIO_PIN_11);
		case 2: return !(GPIOB->IDR & GPIO_PIN_10);
		case 3: return !(GPIOE->IDR & GPIO_PIN_15);
		default:
			break;
		}
		return 0;
	}


private:

};

#endif
