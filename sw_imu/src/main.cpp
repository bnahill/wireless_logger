#include "ch.h"
#include "hal.h"

#include "platform.h"
#include "acquisition.h"

#include "ui/ui.h"
#include "power_management.h"


extern "C"{
	int main(void);
}

using namespace Platform;


int main(void) {
	char test[32];

	halInit();
	chSysInit();
	
	Platform::early_init();

	PowerManagement::enable_rtc_tick();
#if SCHED_TICK_RTC
	PowerManagement::disable_systick();
#endif
	oled.init();
	
	if(!flash.init()){
		evt_log.add("Flash init\n failed!", EventItem::SEVERITY_ERROR);
	}
		
 	if(!guardian1.init()) {
		evt_log.add("Guardian init\nfailed!", EventItem::SEVERITY_ERROR);
	}
	
	evt_log.add("Started up!", EventItem::SEVERITY_NOTE);
	
	Acquisition::init();

	UI::ui.start();

	chThdSleep(TIME_INFINITE);
	return 1;
}
