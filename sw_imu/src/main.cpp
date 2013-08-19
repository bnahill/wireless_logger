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
	halInit();
	chSysInit();
	
	clk_mgr_req_hsi();
	
	Platform::early_init();

#ifdef BOARD_SF
	while(true){
		chThdSleep(MS2ST(200));
		led1.toggle();
		chThdSleep(MS2ST(200));
		led2.toggle();
		chThdSleep(MS2ST(200));
		led3.toggle();
	}
#endif
	
#if SCHED_TICK_RTC
	PowerManagement::disable_systick();
	PowerManagement::enable_rtc_tick();
#endif
	oled.init();
		
//  if(!guardian1.init()) {
// 		evt_log.add("Guardian init\nfailed!", EventItem::SEVERITY_ERROR);
// 	}
	
	Acquisition::init();

	UI::start();
	
	clk_mgr_noreq_hsi();
	
// 	while(1){
// 		led1.toggle();
// 		chThdSleep(MS2ST(1000));
// 	}
	
	chThdSleep(TIME_INFINITE);
	
	return 1;
}
