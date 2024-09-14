#include "sdram.h"
#include "micros.h"

#define SDRAM_TIMEOUT								((uint32_t)0xFFFF)
#define SDRAM_MODEREG_BURST_LENGTH_1      			((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2      			((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4      			((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8      			((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL  		((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED  		((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2      			((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3      			((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD 		((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED	((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE   		((uint16_t)0x0200)


void Sdram::init() {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/** FMC GPIO Configuration
	PF0   ------> FMC_A0
	PF1   ------> FMC_A1
	PF2   ------> FMC_A2
	PF3   ------> FMC_A3
	PF4   ------> FMC_A4
	PF5   ------> FMC_A5
	PC0   ------> FMC_SDNWE
	PC2   ------> FMC_SDNE0
	PC3   ------> FMC_SDCKE0
	PF11   ------> FMC_SDNRAS
	PF12   ------> FMC_A6
	PF13   ------> FMC_A7
	PF14   ------> FMC_A8
	PF15   ------> FMC_A9
	PG0   ------> FMC_A10
	PG1   ------> FMC_A11
	PE7   ------> FMC_D4
	PE8   ------> FMC_D5
	PE9   ------> FMC_D6
	PE10   ------> FMC_D7
	PE11   ------> FMC_D8
	PE12   ------> FMC_D9
	PE13   ------> FMC_D10
	PE14   ------> FMC_D11
	PE15   ------> FMC_D12
	PD8   ------> FMC_D13
	PD9   ------> FMC_D14
	PD10   ------> FMC_D15
	PD14   ------> FMC_D0
	PD15   ------> FMC_D1
	PG2   ------> FMC_A12
	PG4   ------> FMC_BA0
	PG5   ------> FMC_BA1
	PG8   ------> FMC_SDCLK
	PD0   ------> FMC_D2
	PD1   ------> FMC_D3
	PG15   ------> FMC_SDNCAS
	PE0   ------> FMC_NBL0
	PE1   ------> FMC_NBL1
	*/
	/* GPIO_InitStruct */
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
	|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_11|GPIO_PIN_12
	|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	/* GPIO_InitStruct */
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/* GPIO_InitStruct */
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4
	|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	/* GPIO_InitStruct */
	GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
	|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
	|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/* GPIO_InitStruct */
	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14
	|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


	SDRAM_HandleTypeDef hsdram1;
	FMC_SDRAM_TimingTypeDef SdramTiming = {0};

	/** Perform the SDRAM1 memory initialization sequence
	*/
	/* hsdram1.Init */
	/*
	hsdram1.Instance = FMC_SDRAM_DEVICE;
	hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
	hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_11;
	hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
	hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
	hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
	hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_1;
	hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
	hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_DISABLE;
	hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;
	hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
	*/
	/* SdramTiming */
	/*
	SdramTiming.LoadToActiveDelay = 16;
	SdramTiming.ExitSelfRefreshDelay = 16;
	SdramTiming.SelfRefreshTime = 16;
	SdramTiming.RowCycleDelay = 16;
	SdramTiming.WriteRecoveryTime = 16;
	SdramTiming.RPDelay = 16;
	SdramTiming.RCDDelay = 16;

	HAL_SDRAM_Init(&hsdram1, &SdramTiming);
	*/



	/* FMC SDRAM device initialization sequence --------------------------------*/
	/* Step 1 ----------------------------------------------------*/
	/* Timing configuration for 84 Mhz of SD clock frequency (168Mhz/2) */
	/* TMRD: 2 Clock cycles */
	/* 1 clock cycle = 1 / 84MHz = 11.9ns */
	SdramTiming.LoadToActiveDelay    = 2;	//2
	SdramTiming.ExitSelfRefreshDelay = 8;	//7		6?	60nS / 11.9
	SdramTiming.SelfRefreshTime      = 4;	//4
	SdramTiming.RowCycleDelay        = 7;	//7
	SdramTiming.WriteRecoveryTime    = 3;	//3
	SdramTiming.RPDelay              = 2;	//2
	SdramTiming.RCDDelay             = 2;	//2

	/* FMC SDRAM control configuration */
	hsdram1.Instance = FMC_SDRAM_DEVICE;
	hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
	hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_10;
	hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
	hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
	hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
	hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
	hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
	hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
	hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;
	hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;

	/* FMC SDRAM bank initialization */
	HAL_SDRAM_Init(&hsdram1, &SdramTiming);
	Micros::delay(100 * 1000);

	/* SDRAM Init sequence */

	FMC_SDRAM_CommandTypeDef Command;
	/* Configure a clock configuration enable command */
	Command.CommandMode				= FMC_SDRAM_CMD_CLK_ENABLE;
	Command.CommandTarget 			= FMC_SDRAM_CMD_TARGET_BANK1;
	Command.AutoRefreshNumber 		= 1;
	Command.ModeRegisterDefinition 	= 0;

	/* Send command */
	while((HAL_SDRAM_GetState(&hsdram1) & HAL_SDRAM_STATE_BUSY));
	HAL_SDRAM_SendCommand(&hsdram1, &Command, SDRAM_TIMEOUT);

	/* Little delay */
	Micros::delay(500);

	/* Configure a PALL (precharge all) command */
	Command.CommandMode          	= FMC_SDRAM_CMD_PALL;
	Command.CommandTarget          	= FMC_SDRAM_CMD_TARGET_BANK1;
	Command.AutoRefreshNumber      	= 1;
	Command.ModeRegisterDefinition 	= 0;

	/* Send the command */
	while((HAL_SDRAM_GetState(&hsdram1) & HAL_SDRAM_STATE_BUSY));
	HAL_SDRAM_SendCommand(&hsdram1, &Command, SDRAM_TIMEOUT);

	/* Configure a Auto-Refresh command */
	Command.CommandMode            	= FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	Command.CommandTarget          	= FMC_SDRAM_CMD_TARGET_BANK1;
	Command.AutoRefreshNumber      	= 8;	//4 ?
	Command.ModeRegisterDefinition 	= 0;

	/* Send the command */
	while((HAL_SDRAM_GetState(&hsdram1) & HAL_SDRAM_STATE_BUSY));
	HAL_SDRAM_SendCommand(&hsdram1, &Command, SDRAM_TIMEOUT);

	/* Configure a load Mode register command */
	Command.CommandMode            	= FMC_SDRAM_CMD_LOAD_MODE;
	Command.CommandTarget          	= FMC_SDRAM_CMD_TARGET_BANK1;
	Command.AutoRefreshNumber      	= 1;
	//Command.ModeRegisterDefinition 	= (uint32_t)0x0231;
	Command.ModeRegisterDefinition 	= (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1 | SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL | SDRAM_MODEREG_CAS_LATENCY_3 | SDRAM_MODEREG_OPERATING_MODE_STANDARD | SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

	/* Wait until the SDRAM controller is ready */
	/* Send the command */
	while((HAL_SDRAM_GetState(&hsdram1) & HAL_SDRAM_STATE_BUSY));
	HAL_SDRAM_SendCommand(&hsdram1, &Command, SDRAM_TIMEOUT);

	/* Step 6: Set the refresh rate counter */
	// 64mS / 8192 cycles = 7.8uS
	// refresh count = 7.8uS * 84MHz - 20 = 635
	while((HAL_SDRAM_GetState(&hsdram1) & HAL_SDRAM_STATE_BUSY));
	HAL_SDRAM_ProgramRefreshRate(&hsdram1, 635);	//680 ?

	/* Little delay */
	while((HAL_SDRAM_GetState(&hsdram1) & HAL_SDRAM_STATE_BUSY));
	Micros::delay(500);
}
