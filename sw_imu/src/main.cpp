#include "ch.h"
#include "hal.h"

#include "platform.h"
#include "acquisition.h"

#include "ui/ui.h"

extern "C"{
	int main(void);
}


int main(void) {
	halInit();
	chSysInit();
	
	Platform::early_init();
	
	oled.init();
	
	if(!rf1.init())
		while(1);
	
	Acquisition::init();
	
	UI::ui.start();
	
	while(true){
		chThdSleep(MS2ST(10));
	}
	
	chThdSleep(TIME_INFINITE);
}
