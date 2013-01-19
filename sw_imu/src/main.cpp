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
	
	// Turn the 
	reg1.high_power(true);
	
	ui_init();
	
	Acquisition::init();
	
	UI::ui.start();
	
	while(true){
		chThdSleep(MS2ST(10));
		led1.clear();
		
	}
	
	chThdSleep(TIME_INFINITE);
}

void ui_init(){
	Platform::EXTInit();
	
	oled.init();
}