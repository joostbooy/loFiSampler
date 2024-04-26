#include "gate.h"

Gate Gate;

void Gate::init() {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/**SPI4 GPIO Configuration
	PB14     ------> Gate in 1
	PB11     ------> Gate in 2
	PB10     ------> Gate in 3
	PE15     ------> Gate in 4
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}
