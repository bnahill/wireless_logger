#include "ch.h"
#include "hal.h"

#include "platform.h"
#include "acquisition.h"

#include "ui/ui.h"
#include "power_management.h"

#include "yaffsfs.h"

extern "C"{
	int main(void);
}

using namespace Platform;

int main(void) {
	halInit();
	chSysInit();

	Platform::early_init();

	oled.init();
	
	flash.init();

	if(!guardian1.init())
		while(1);

	Acquisition::init();

	UI::ui.start();

	chThdSleep(TIME_INFINITE);
}
