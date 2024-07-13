#include "dac.h"

Dac* Dac::dac_;

void Dac::init() {

	dac_ = this;

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/**I2S3 GPIO Configuration
    PA15     ------> I2S3_WS
    PC10     ------> I2S3_CK
    PC12     ------> I2S3_SD
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


	I2S_HandleTypeDef hi2s3;
	hi2s3.Instance = SPI3;
    hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
    hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
    hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
    hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
    hi2s3.Init.AudioFreq = kSampleRate * kNumChannels;;
    hi2s3.Init.CPOL = I2S_CPOL_LOW;
    hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
	//hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
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
}

void Dac::start(void (*callback)(Buffer*, size_t)) {
	callback_ = callback;
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

extern "C" {
	void DMA1_Channel5_IRQHandler(void) {
		uint32_t flags = DMA1->LISR;
		DMA1->LIFCR |= DMA_HIFCR_CTCIF5 | DMA_HIFCR_CHTIF5;

		if (flags & DMA_HISR_TCIF5) {
			Dac::dac_->fill(1);
		} else if (flags & DMA_HISR_HTIF5) {
			Dac::dac_->fill(0);
		}
	}
}
