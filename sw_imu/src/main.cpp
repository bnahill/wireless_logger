#include "ch.h"
#include "hal.h"

#include "platform.h"
#include "acquisition.h"

#include "ui/ui.h"
#include "power_management.h"

#include "coffee/cfs-coffee.h"

extern "C"{
	int main(void);
}

using namespace Platform;

int main(void) {
	halInit();
	chSysInit();

	Platform::early_init();

	oled.init();
	
	if(!flash.init())
		while(1);

 	if(!guardian1.init())
		while(1);
	
	Acquisition::init();

	UI::ui.start();

	chThdSleep(TIME_INFINITE);
	return 1;
}
