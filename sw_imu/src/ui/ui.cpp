#include "ui/ui.h"
#include "ui/mainmenu.h"

using namespace Platform;



bool UI::is_suspended = false;
bool UI::suspend_enabled = true;

Thread * UI::thread = nullptr;
Thread * UI::monitor_thread = nullptr;
	
VirtualTimer UI::timer;

WORKING_AREA(UI::MonitorThread, monitor_stack_size);
	//! Stack area for the thread
WORKING_AREA(UI::UIThread, stack_size);

msg_t UI::run(void * nothing){
	MainMenu menu;
	button[0].enable();
	button[1].enable();
	button[2].enable();
	
	button[0].clear_callbacks();
	button[1].clear_callbacks();
	button[2].clear_callbacks();
	
	button[0].set_press_handler((button_t::button_handler)handle_right, nullptr);
	button[1].set_press_handler((button_t::button_handler)handle_select, nullptr);
	button[2].set_press_handler((button_t::button_handler)handle_left, nullptr);
	start_suspend_timer();
	menu.exec();
	return 0;
}

void UI::wait_for_button(ui_event_t evt, bool allow_sleep){
	ui_event_t recvd;
	chEvtGetAndClearEvents(MASK_LEFT | MASK_RIGHT | MASK_SELECT);
	if(allow_sleep){
		while(!(evt & chEvtWaitOne(evt | MASK_SUSPEND))){
			// This must actually be a suspend event
			suspend();
			chEvtWaitOne(MASK_RESUME);
			resume();
			chEvtGetAndClearEvents(ALL_EVENTS);
		}
	} else {
		chEvtWaitAny(evt);
	}
	chEvtGetAndClearEvents(MASK_LEFT | MASK_RIGHT | MASK_SELECT);
}

msg_t UI::run_monitor(void * nothing){
	char text[10];
	EventItem * item;
	bool ev_note, ev_warn, ev_err;
	uint32_t column;
	
	oled.fb.draw_horizontal_mask(0, 0x7F, 100, 1);
	
	while(!chThdShouldTerminate()){
		rtc1::get_time_text(text);
		oled.fb.write_text<SmallFont>(text,0,0);
		
		// Display event status
		ev_warn = false;
		ev_err = false;
		ev_note = false;
		for(item = evt_log.items_head; item; item = evt_log.next(item)){
			ev_note |= (item->severity == EventItem::SEVERITY_NOTE);
			ev_warn |= (item->severity == EventItem::SEVERITY_WARNING);
			ev_err |= (item->severity == EventItem::SEVERITY_ERROR);
		}
		
		oled.fb.clear_area(0, 0, 101);
		column = 103;
		if(ev_note)
			column = oled.fb.write_text<SmallFont>("n", 0, column) + 2;
		if(ev_warn)
			column = oled.fb.write_text<SmallFont>("w", 0, column) + 2;
		if(ev_err)
			oled.fb.write_text<SmallFont>("e", 0, column);
		
		if(!is_suspended){
			oled.update();
		}
		chThdSleep(MS2ST(500));
		//led1.assign(reg1.is_charging());
	}
	return 0;
}

eventmask_t UI::handle_evt(eventmask_t evt){
	switch(evt){
	case MASK_RESUME:
		resume();
		break;
	case MASK_SUSPEND:
		suspend();
		break;
	default:
		break;
	}
	return evt;
}

void UI::start(){
	if(thread){
		chThdTerminate(thread);
		chThdWait(thread);
	}
	thread = chThdCreateStatic(&UIThread, stack_size, NORMALPRIO - 2,
								(tfunc_t)run, nullptr);
	monitor_thread = chThdCreateStatic(&MonitorThread, monitor_stack_size,
	                                   NORMALPRIO - 2,
	                                   (tfunc_t)run_monitor, nullptr);
}

