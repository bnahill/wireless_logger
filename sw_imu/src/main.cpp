#include "ch.h"
#include "hal.h"

#include "platform.h"
#include "acquisition.h"

#include "ui/ui.h"
#include "power_management.h"

extern "C"{
	int main(void);
}


int main(void) {
	halInit();
	chSysInit();
	
	Platform::early_init();
	
	oled.init();
	
	if(!guardian1.init())
		while(1);
	
	Acquisition::init();
	
	UI::ui.start();
	
	chThdSleep(TIME_INFINITE);
}
