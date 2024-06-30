#ifndef adc_h
#define adc_h

#include "stmf4lib.h"


class Adc {

public:
	void init();

	inline bool ready() {
		return ADC1->SR & ADC_SR_EOC;
	}

	// circuit is inverted
	inline uint16_t read() {
		return 4095 - ADC1->DR;
	}

	inline uint8_t channel(){
		return channel_;
	}

	inline void convert_next_channel() {
		if (++channel_ >= kNumChannels) {
			channel_ = 0;
		}

		switch (channel_)
		{
		case 0:	ADC1->SQR3 = ADC_CHANNEL_8;
			break;
		case 1:	ADC1->SQR3 = ADC_CHANNEL_9;
			break;
		case 2:	ADC1->SQR3 = ADC_CHANNEL_14;
			break;
		case 3:	ADC1->SQR3 = ADC_CHANNEL_15;
			break;
		case 4:	ADC1->SQR3 = ADC_CHANNEL_15;
			break;
		case 5:	ADC1->SQR3 = ADC_CHANNEL_4;
			break;
		default:
			break;
		}

		ADC1->CR2 |= ADC_CR2_SWSTART;
	}

private:
	uint8_t channel_ = 0;
	static const size_t kNumChannels = 5;
};

extern Adc adc;

#endif
