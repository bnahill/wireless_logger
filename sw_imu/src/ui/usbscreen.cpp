#include "ui/usbscreen.h"

using namespace Platform;

void USBScreen::exec(){
	eventmask_t evt;
	
	oled.fb.clear_area(1);
	oled.fb.write_text_centered<SmallFont>("USB MODE ACTIVE", 2,
	                                       oled.fb.num_columns/2,
	                                       oled.fb.num_columns);
	oled.fb.write_text_centered<SmallFont>("(select to quit)", 3,
	                                       oled.fb.num_columns/2,
	                                       oled.fb.num_columns);
	oled.update();

	// Launch the USB terminal
	term.launch();
	
	chEvtGetAndClearEvents(ALL_EVENTS);

	while(!chThdShouldTerminate()){
		evt = chEvtWaitOneTimeout(ALL_EVENTS, MS2ST(5));
		evt = UI::handle_evt(evt);
		
		switch(evt){
		case 0:
			continue;
		case UI::MASK_ABORT:
			terminate();
			return;
		case UI::MASK_SELECT:
			terminate();
			return;
		case UI::MASK_SUSPEND:
			evt = chEvtWaitOne(UI::MASK_RESUME);
			chEvtGetAndClearEvents(ALL_EVENTS);
			UI::handle_evt(UI::MASK_RESUME);
			break;
		default:
			break;
		}
	}
	
	terminate();
}

msg_t USBScreen::terminate(){
	return term.terminate();
}

