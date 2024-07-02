#ifndef Timer_h
#define Timer_h

#include "stm32f4xx.h"

class Timer {

public:
	void start_2(uint32_t freq);
	void start_3(uint32_t freq);
	void start_4(uint32_t freq);

private:

};

extern Timer timer;

#endif
