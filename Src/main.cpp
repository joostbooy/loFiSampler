#include "system.h"
#include "micros.h"
#include "debug.h"
#include "matrix.h"
#include "adc.h"
#include "dac.h"
#include "display.h"
#include "disk.h"
#include "gate.h"
#include "sdram.h"

#include "settings.h"
#include "engine.h"
#include "ui.h"

Adc adc;
Dac dac;
Usb usb;
Gate gate;
Uart uart;
Sys sys;
Sdram sdram;
Matrix matrix;
Display display;

Ui ui;
Engine engine;
Settings settings;

extern "C" {

	void NMI_Handler() { }
	void HardFault_Handler() { while (1); }
	void MemManage_Handler() { while (1); }
	void BusFault_Handler() { while (1); }
	void UsageFault_Handler() { while (1); }
	void SVC_Handler() { }
	void DebugMon_Handler() { }
	void PendSV_Handler() { }
	void SysTick_Handler(void) { }

	// 1Khz
	void TIM4_IRQHandler(void) {
		if (!(TIM4->SR & TIM_IT_UPDATE)) {
			return;
		}
		TIM4->SR = ~TIM_IT_UPDATE;
		ui.poll();
	}

} //extern "C"


int main(void)
{
	sys.init();

	Micros::init();
	Debug::init();

	dac.init();
	uart.init();
	gate.init();
	usb.init();
	adc.init();
	matrix.init();
	display.init();
	sdram.init();
	sdio.init();
	disk.init();

	settings.init(&sdram);
	engine.init(&settings, &uart, &usb, &gate);
	ui.init(&settings, &engine, &matrix, &display);

	// start
	engine.fill(&dac.buffer_[0], Dac::kBlockSize);

	while (1) {
		ui.process();
	}
}
