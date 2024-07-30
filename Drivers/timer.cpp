#include "timer.h"

// Engine
void Timer::start_3(uint32_t freq){
	TIM_HandleTypeDef TimHandle;

	TimHandle.Instance               = TIM3;
	TimHandle.Init.Period            = (F_CPU / freq / 2) - 1;
	TimHandle.Init.Prescaler         = 0;
	TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	TimHandle.Init.RepetitionCounter = 0;
	TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	HAL_TIM_Base_Init(&TimHandle);
	HAL_TIM_Base_Start_IT(&TimHandle);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

// Ui
void Timer::start_2(uint32_t freq){
	TIM_HandleTypeDef htim2;
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = (F_CPU / 1000 / 2) - 1;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	HAL_TIM_Base_Init(&htim2);
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

	HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}
