#include "ui/usbscreen.h"

void USBScreen::exec(){
	eventmask_t evt;
	
	oled.fb.clear_area(1);
	oled.fb.write_text_centered<SmallFont>("USB MODE ACTIVE", 2,
	                                       oled.fb.num_columns/2,
	                                       oled.fb.num_columns);
	oled.update();
	thread = chThdCreateStatic(USBScreenThread, stack_size, LOWPRIO,
	                           (tfunc_t) start_thread, (void *)this);
	
	chEvtGetAndClearEvents(ALL_EVENTS);

	while(!chThdShouldTerminate()){
		evt = chEvtWaitOneTimeout(ALL_EVENTS, MS2ST(5));
		switch(evt){
		case 0:
			continue;
		case UI::MASK_ABORT:
			terminate();
			return;
		case UI::MASK_SELECT:
			terminate();
			return;
		default:
			break;
		}
	}
	
	terminate();
}

msg_t USBScreen::terminate(){
	chThdTerminate(thread);
	return chThdWait(thread);
}

void USBScreen::thread_action(){
	Euclidean3_f32 m;
	
	usbserial1.init();
	while(!chThdShouldTerminate()){
		chThdSleep(MS2ST(10));
		acc1.get_reading(m);
		chprintf(usbserial1.stream(),
		         "%f -- %f -- %f\r", m.x, m.y, m.z);
	}
	usbserial1.stop();
}
