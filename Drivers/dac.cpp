#include "dac.h"
#include "debug.h"

Dac* Dac::dac_;
bool Dac::first_frame_;
uint16_t Dac::dma_buffer_[Dac::kDmaBufferSize];
//uint16_t Dac::dma_buffer_[Dac::kDmaBufferSize] __attribute__((section(".dtcm")));

uint16_t Dac::left_channel_bits_[kNumStereoChannels] = {
	0x0300 | (0 << 4),
	0x0300 | (2 << 4),
	0x0300 | (4 << 4),
	0x0300 | (6 << 4)
};

uint16_t Dac::right_channel_bits_[kNumStereoChannels] = {
	0x0300 | (1 << 4),
	0x0300 | (3 << 4),
	0x0300 | (5 << 4),
	0x0300 | (7 << 4)
};

// DMA1 stream 5 Channel 0
const uint32_t kChannel0				= (0 << 25);
const uint32_t kPriorityVeryHigh		= (3 << 16);
const uint32_t kEnableMemoryIncrement	= (1 << 10);
const uint32_t kEnableCircularMode		= (1 << 8);
const uint32_t kMemoryToPeripheral		= (1 << 6);
const uint32_t kEnable_TC_interupt		= (1 << 4);
const uint32_t kEnable_HTC_interupt		= (1 << 3);
const uint32_t kPeripheralSize_16bit	= (1 << 11);

void Dac::init() {

	dac_ = this;

	/**I2S3 GPIO Configuration
	PA15     ------> I2S3_WS
	PC10     ------> I2S3_CK
	PC12     ------> I2S3_SD
	*/
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	I2S_HandleTypeDef hi2s3;
	hi2s3.Instance = SPI3;
	hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
	hi2s3.Init.Standard = I2S_STANDARD_PHILLIPS;
	hi2s3.Init.DataFormat = I2S_DATAFORMAT_32B;
	hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
	hi2s3.Init.AudioFreq = SAMPLE_RATE * (kNumStereoChannels * 2);
	hi2s3.Init.CPOL = I2S_CPOL_HIGH;
	hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
	HAL_I2S_Init(&hi2s3);
	__HAL_I2S_ENABLE(&hi2s3);

	// Init DMA
	DMA1_Stream5->CR &= ~DMA_SxCR_EN;
	while (DMA1_Stream5->CR & DMA_SxCR_EN);

	DMA1_Stream5->CR = kChannel0 | kPriorityVeryHigh | kEnableMemoryIncrement | kEnableCircularMode | kMemoryToPeripheral | \
	kEnable_TC_interupt | kEnable_HTC_interupt | kPeripheralSize_16bit;

	DMA1_Stream5->FCR &= ~DMA_SxFCR_DMDIS;

	DMA1->HIFCR |= DMA_HIFCR_CTCIF5 | DMA_HIFCR_CHTIF5 | DMA_HIFCR_CTEIF5 | DMA_HIFCR_CDMEIF5 | DMA_HIFCR_CFEIF5;
	DMA1_Stream5->PAR = reinterpret_cast<uint32_t>(&SPI3->DR);
	DMA1_Stream5->M0AR = reinterpret_cast<uint32_t>(&dma_buffer_[0]);
	DMA1_Stream5->NDTR = kDmaBufferSize;
	DMA1_Stream5->CR |= DMA_SxCR_EN;
	SPI3->CR2 |= SPI_CR2_TXDMAEN;

	//SCB_InvalidateDCache_by_Addr(reinterpret_cast<uint32_t*>(&dma_buffer_[0]), kDmaBufferSize / 2);
	//SCB_CleanDCache_by_Addr(reinterpret_cast<uint32_t*>(&dma_buffer_[0]), kDmaBufferSize / 2);
}

void Dac::start(void (*callback)(Channel*, size_t)) {
	first_frame_ = true;
	callback_ = callback;
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

void Dac::fill(const size_t offset) {
	callback_(channel_, BLOCK_SIZE);

	uint16_t *ptr = &dma_buffer_[offset * (kDmaBufferSize / 2)];

	if (first_frame_) {
		for (size_t i = 0; i < BLOCK_SIZE; ++i) {
			for (size_t chn = 0; chn < kNumStereoChannels * 2; ++chn) {
				*ptr++ = 0x090A;
				*ptr++ = 0x0000;
				*ptr++ = 0x0000;
				*ptr++ = 0x0000;
			}
		}
		first_frame_ = false;
	} else {
		for (size_t i = 0; i < BLOCK_SIZE; ++i) {
			for (size_t chn = 0; chn < kNumStereoChannels; ++chn) {
				uint16_t left = channel_[chn].left[i] + 32768;
				uint16_t right = channel_[chn].right[i] + 32768;

				*ptr++ = left_channel_bits_[chn] | (left >> 12);
				*ptr++ = left << 4;
				*ptr++ = 0;
				*ptr++ = 0;

				*ptr++ = right_channel_bits_[chn] | (right >> 12);
				*ptr++ = right << 4;
				*ptr++ = 0;
				*ptr++ = 0;
			}
		}
	}
}

extern "C" {
	void DMA1_Stream5_IRQHandler(void) {
		uint32_t flags = DMA1->HISR;
		DMA1->HIFCR |= DMA_HIFCR_CTCIF5 | DMA_HIFCR_CHTIF5;

		if (flags & DMA_HISR_TCIF5) {
			Dac::dac_->fill(1);
		} else if (flags & DMA_HISR_HTIF5) {
			Dac::dac_->fill(0);
		}
	}
}
