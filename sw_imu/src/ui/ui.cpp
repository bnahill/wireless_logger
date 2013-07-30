#include "ui/ui.h"
#include "ui/mainmenu.h"

UI UI::ui;

using namespace Platform;

msg_t UI::run(){
	MainMenu menu;
	button[0].enable();
	button[1].enable();
	button[2].enable();
	
	button[0].clear_callbacks();
	button[1].clear_callbacks();
	button[2].clear_callbacks();
	
	button[0].set_press_handler((button_t::button_handler)handle_right,
	                            reinterpret_cast<uint32_t *>(this));
	button[1].set_press_handler((button_t::button_handler)handle_select,
	                            reinterpret_cast<uint32_t *>(this));
	button[2].set_press_handler((button_t::button_handler)handle_left,
	                            reinterpret_cast<uint32_t *>(this));
	start_suspend_timer();
	menu.exec();
	return 0;
}

msg_t UI::run_monitor(){
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
								(tfunc_t)start_thread, this);
	monitor_thread = chThdCreateStatic(&MonitorThread, monitor_stack_size,
	                                   NORMALPRIO - 2,
	                                   (tfunc_t)start_monitor_thread, this);
}

