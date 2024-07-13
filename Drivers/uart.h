#ifndef uart_h
#define uart_h

#include "stm32f7xx.h"

class Uart {

public:

	void init();

	void write(uint8_t data) {
		USART6->TDR = data;
	}

	bool writeable() {
		return USART6->TDR & UART_FLAG_TXE;
	}

	uint8_t read() {
		return USART6->RDR;
	}

	bool readable() {
		return USART6->RDR & UART_FLAG_RXNE;
	}

};

#endif
