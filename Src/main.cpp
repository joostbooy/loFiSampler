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
#include "timer.h"
#include "sdmmc.h"

#include "disk.h"
#include "settings.h"
#include "engine.h"
#include "ui.h"

Adc adc;
Dac dac;
Usb usb;
Gate gate;
Uart uart;
Sys sys;
Timer timer;
Sdram sdram;
Sdmmc sdmmc;
Matrix matrix;
Display display;

Disk disk;
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

	// Clock update freq (4kHz)
	void TIM3_IRQHandler(void) {
		if (!(TIM3->SR & TIM_IT_UPDATE)) {
			return;
		}
		TIM3->SR = ~TIM_IT_UPDATE;
		engine.tick();
	}

	// 1Khz
	void TIM7_IRQHandler(void) {
		if (!(TIM7->SR & TIM_IT_UPDATE)) {
			return;
		}
		TIM7->SR = ~TIM_IT_UPDATE;
		ui.poll();
	}

} //extern "C"

void fill(Dac::Buffer *buffer, const size_t size) {
	engine.fill(buffer, size);
}

int main(void)
{
	// Init drivers
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
	sdmmc.init();

	// Init engine, settings & ui
	disk.init(&sdmmc);
	settings.init(&sdram, &disk);
	engine.init(&settings, &uart, &usb, &gate);
	ui.init(&settings, &engine, &matrix, &display);

	// Start timers
	dac.start(fill);
	timer.start_3(CLOCK_ISR_FREQ);
	timer.start_7(1000);

	while (1) {
		ui.process();
	}
}
