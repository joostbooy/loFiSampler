#include "sdio.h"

Sdio sdio;

void Sdio::init() {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	//powerOff();

	/**SDMMC2 GPIO Configuration
	PD6     ------> SDMMC2_CK
	PD7     ------> SDMMC2_CMD
	PG9     ------> SDMMC2_D0
	PG10     ------> SDMMC2_D1
	PG11     ------> SDMMC2_D2
	PG12     ------> SDMMC2_D3
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF11_SDMMC2;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF11_SDMMC2;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_SDMMC2;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	// Enable dma interrupt
	HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
}


// DMA2 steam 0 Channel 4
const uint32_t kChannel4				= (4 << 25);
const uint32_t kMemoryBurst_inc4		= (1 << 23);
const uint32_t kPeripheralBurst_inc4	= (1 << 21);
const uint32_t kPriorityVeryHigh		= (3 << 16);
const uint32_t kMemorySize_32bit		= (2 << 13);
const uint32_t kPeripheralSize_32bit	= (2 << 11);
const uint32_t kEnableMemoryIncrement	= (1 << 10);
const uint32_t kEnableFifo				= (1 << 8);
const uint32_t kMemoryToPeripheral		= (1 << 6);
const uint32_t kPeripheralFlowControl	= (1 << 5);
const uint32_t kEnable_TC_interupt		= (1 << 4);

void Sdio::init_dma(uint32_t buffer, DmaType dmaType) {
	DMA2_Stream0->CR &= ~DMA_SxCR_EN;			// disable stream & wait
	while (DMA2_Stream0->CR & DMA_SxCR_EN) {};

	uint32_t direction = 0;
	if (dmaType == MEM_TO_SD) {
		direction =	kMemoryToPeripheral;
	} else if (dmaType == SD_TO_MEM) {
		direction =	0;
	}

	DMA2_Stream0->CR = 0;
	DMA2_Stream0->CR = (kChannel4 | kMemoryBurst_inc4 | kPeripheralBurst_inc4 | kPriorityVeryHigh | kMemorySize_32bit | kPeripheralSize_32bit | kEnableMemoryIncrement | kPeripheralFlowControl | direction | kEnable_TC_interupt);

	DMA2_Stream0->FCR |= DMA_SxFCR_DMDIS;	// disable direct mode (enables fifo mode)
	DMA2_Stream0->FCR |= (3 << 0); 			// fifo treshold full

	DMA2->LIFCR |= DMA_LIFCR_CTCIF0 | DMA_LIFCR_CHTIF0 | DMA_LIFCR_CTEIF0 | DMA_LIFCR_CDMEIF0 | DMA_LIFCR_CFEIF0;
	DMA2_Stream0->PAR = reinterpret_cast<uint32_t>(&SDMMC2->FIFO);
	DMA2_Stream0->M0AR = buffer;
	DMA2_Stream0->NDTR = 0;
	DMA2_Stream0->CR |= DMA_SxCR_EN;		// enable stream

	sdio.lock_dma();
}

extern "C" {
	void DMA2_Stream0_IRQHandler(void) {
		uint32_t flags = DMA2->LISR;
		DMA2->LIFCR |= DMA_LIFCR_CTCIF0 | DMA_LIFCR_CHTIF0;

		if (flags & DMA_LISR_TCIF0) {
			sdio.unlock_dma();
		}
	}
}
