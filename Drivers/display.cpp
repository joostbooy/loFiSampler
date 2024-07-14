#include "display.h"
#include "micros.h"

Display* Display::display_;

void Display::init() {
	display_ = this;

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/**GPIO Configuration
	PC13     ------> RESET
	PC14     ------> CS
	PC15     ------> CD
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


	/**SPI2 GPIO Configuration
    PF7     ------> SPI5_SCK
    PF8     ------> SPI5_MISO
    PF9     ------> SPI5_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	// SPI 5
	SPI_HandleTypeDef hspi5;
	hspi5.Instance = SPI5;
    hspi5.Init.Mode = SPI_MODE_MASTER;
    hspi5.Init.Direction = SPI_DIRECTION_2LINES;
    hspi5.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi5.Init.CLKPolarity = SPI_POLARITY_HIGH;
    hspi5.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi5.Init.NSS = SPI_NSS_SOFT;
    hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi5.Init.CRCPolynomial = 7;
    // hspi5.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    // hspi5.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
    HAL_SPI_Init(&hspi5);
	__HAL_SPI_ENABLE(&hspi5);

	//DMA
	DMA2_Stream4->CR &= ~DMA_SxCR_EN;	// disable stream & wait
	while ((DMA2_Stream4->CR) & DMA_SxCR_EN);

	DMA2_Stream4->CR = (0 << 25) | DMA_SxCR_MINC | DMA_SxCR_DIR_0 | DMA_SxCR_TCIE | DMA_SxCR_DMEIE;
	DMA2_Stream4->FCR &= ~DMA_SxFCR_DMDIS;

	dma_busy_ = false;

	// Select, Reset & wait
	select();

	GPIOC->BSRR = GPIO_PIN_13 << 16;
	Micros::delay(150 * 1000);
	GPIOC->BSRR = GPIO_PIN_13;
	Micros::delay(150 * 1000);

	// Start lcd configuration
	sendCommand(0xfd, 0x12);		// Set Command Lock (MCU protection status) | 0x12 = reset
	sendCommand(0xae);				// Set Display Off
	sendCommand(0xb3, 0xd1);		// Set Front Clock Divider / Oscillator Frequency
	sendCommand(0xca, 0x3f);		// Set MUX Ratio | 0x3f = 64mux
	sendCommand(0xa2, 0x00);		// Set Display Offset | 0x00 = reset
	sendCommand(0xa1, 0x00);		// Set Display Start Line, 0x00 = register
	sendCommand(0xa0, 0x14, 0x11);	// Set Re-map and Dual COM Line mode
	// 0x14 = Reset except Enable Nibble Re-map, Scan from COM[N-1] to COM0, where N is the Multiplex ratio
	// 0x11 = Reset except Enable Dual COM mode (MUX = 63)
	sendCommand(0xb5, 0x00);		// Set GPIO
	sendCommand(0xab, 0x00);		// Function Selection | 0x01 = reset = Enable internal VDD regulator
	sendCommand(0xb4, 0xa0, 0xb5);	// Display Enhancement A | 0xa0 = Enable external VSL |  0xb5 = Normal (reset)
	sendCommand(0xc1, 0x9f);		// Set Contrast Current
	sendCommand(0xc7, 0x0f);		// Master Contrast Current Control | 0x0f = no change
	sendCommand(0xb9);				// Select Default Linear Gray Scale table
	sendCommand(0xb1, 0x74);		// Set Phase Length | 0xe2 = Phase 1 period (reset phase length) = 5 DCLKs, Phase 2 period (first pre-charge phase length) = 14 DCLKs
	sendCommand(0xd1, 0xa2, 0x20);	// Display Enhancement B | 0xa2 = Normal (reset) | 0x20 = n/a
	sendCommand(0xbb, 0x17);		// Set Pre-charge voltage | 0x1f = 0.6 * VCC
	sendCommand(0xb6, 0x08);		// Set Second Precharge Period | 0x08 = 8 dclks
	sendCommand(0xbe, 0x07);		// Set VCOMH | 0x07 = = 0.86 x VCC
	sendCommand(0xa6);				// Set Display Mode = Normal Display
	sendCommand(0xa9);				// Exit Partial Display
	Micros::delay(10 * 1000);
	sendCommand(0xaf);				// Set Sleep mode OFF (Display ON),
	Micros::delay(50 * 1000);

	// Start dma interrupt
	HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
}

extern "C" {
	void DMA2_Stream4_IRQHandler(void) {
		uint32_t flags = DMA2->HISR;
		DMA2->HIFCR |= DMA_HIFCR_CTCIF4 | DMA_HIFCR_CHTIF4;

		if (flags & DMA_HISR_TCIF4) {
			Display::display_->unlock_dma();
		}
	}
}
