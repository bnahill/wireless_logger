#include "ch.h"
#include "hal.h"

#include "platform.h"
#include "acquisition.h"

#include "ui/ui.h"

extern "C"{
	int main(void);
}

void ui_init();


int main(void) {
	halInit();
	chSysInit();
	ui_init();
	
	reg1.high_power(true);
	
	Acquisition::init();
	
	oled.init();
	
	usbserial1.init();
	
	UI::ui.start();
	
	chThdSleep(TIME_INFINITE);
}

void ui_init(){
	Platform::EXTInit();
	button[0].enable();
	button[1].enable();
	button[2].enable();
}