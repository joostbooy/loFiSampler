#include "dac.h"

Dac* Dac::dac_;

void Dac::spi_write(uint8_t command, uint8_t address, uint16_t data, uint8_t function) {
	GPIOA->BSRR = GPIO_PIN_15 << 16;

	spi_write(command);
	spi_write((address << 4) | (data >> 12));
	spi_write(data >> 4);
	spi_write((data & 0xF) << 4 | function);

	GPIOA->BSRR = GPIO_PIN_15;

	Micros::delay(50);
}

void Dac::spi_write(uint8_t data) {
	volatile uint8_t dummy;

	while (!(SPI3->SR & SPI_FLAG_TXE));
	SPI3->DR = data;

	while (!(SPI3->SR & SPI_FLAG_RXNE));
	dummy = SPI3->DR;
}

void Dac::init() {
	dac_ = this;

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/**I2S3 GPIO Configuration
	PA15     ------> I2S3_WS
	PC10     ------> I2S3_CK
	PC12     ------> I2S3_SD
	*/


/*
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
*/
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	SPI_HandleTypeDef hspi3;
	hspi3.Instance = SPI3;
	hspi3.Init.Mode = SPI_MODE_MASTER;
	hspi3.Init.Direction = SPI_DIRECTION_2LINES;
	hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi3.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi3.Init.NSS = SPI_NSS_SOFT;
	hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi3.Init.CRCPolynomial = 10;
	HAL_SPI_Init(&hspi3);
	__HAL_SPI_ENABLE(&hspi3);

	// sync_pin HIGH
	GPIOA->BSRR = GPIO_PIN_15;
	// reset
	spi_write(7, 0, 0, 0);
	// load clear code register
	spi_write(5, 0, 0, 3);
	// enable internal reference
	spi_write(8, 0, 0, 1);
	// power up
	spi_write(4, 0, 0, 0xFF);

	// Stop SPI
	//	HAL_SPI_DeInit(&hspi3);
	//	__HAL_SPI_DISABLE(&hspi3);

	// Start I2S setup
	/*
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	I2S_HandleTypeDef hi2s3;
	hi2s3.Instance = SPI3;
	hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
	hi2s3.Init.Standard = I2S_STANDARD_PCM_SHORT;
	hi2s3.Init.DataFormat = I2S_DATAFORMAT_32B;
	hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
	hi2s3.Init.AudioFreq = SAMPLE_RATE * (kNumStereoChannels * 2);
	hi2s3.Init.CPOL = I2S_CPOL_LOW;
	hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
	HAL_I2S_Init(&hi2s3);
	__HAL_I2S_ENABLE(&hi2s3);


	DMA_HandleTypeDef hdma_spi3_tx;
	hdma_spi3_tx.Instance = DMA1_Stream5;
	hdma_spi3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_spi3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_spi3_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_spi3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_spi3_tx.Init.MemDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_spi3_tx.Init.Mode = DMA_CIRCULAR;
	hdma_spi3_tx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	HAL_DMA_Init(&hdma_spi3_tx);
	__HAL_LINKDMA(&hi2s3, hdmatx, hdma_spi3_tx);
	HAL_I2S_Transmit_DMA(&hi2s3, (uint16_t*)dma_buffer_, kDmaBufferSize);
	*/
}

void Dac::start(void (*callback)(Channel*, size_t)) {
	callback_ = callback;
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

extern "C" {
	void DMA1_Stream5_IRQHandler(void) {
		uint32_t flags = DMA1->LISR;
		DMA1->LIFCR |= DMA_HIFCR_CTCIF5 | DMA_HIFCR_CHTIF5;

		if (flags & DMA_HISR_TCIF5) {
			Dac::dac_->fill(1);
		} else if (flags & DMA_HISR_HTIF5) {
			Dac::dac_->fill(0);
		}
	}
}
