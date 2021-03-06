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

	Platform::early_init();
	
	clk_mgr_req_hsi();




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

	//Acquisition::require_sources(Acquisition::SRC_ACC1 |
	//							 Acquisition::SRC_MAG1 |
	//							 Acquisition::SRC_GYRO1);
	clk_mgr_noreq_hsi();

// 	while(1){
// 		led1.toggle();
// 		chThdSleep(MS2ST(1000));
// 	}

#ifdef BOARD_SF
	while(true){
		chThdSleep(TIME_INFINITE);
//		chThdSleep(MS2ST(200));
//		led1.toggle();
// 		chThdSleep(MS2ST(200));
// 		led2.toggle();
// 		chThdSleep(MS2ST(200));
// 		led3.toggle();
	}
#endif

	return 1;
}
