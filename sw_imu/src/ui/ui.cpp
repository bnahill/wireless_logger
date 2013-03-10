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
	
	while(!chThdShouldTerminate()){
		rtc1::get_time_text(text);
		oled.fb.write_text<SmallFont>(text,0,0);
		oled.update();
		led1.assign(reg1.is_charging());
		chThdSleep(MS2ST(500));
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

