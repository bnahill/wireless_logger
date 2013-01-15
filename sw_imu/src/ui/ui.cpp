#include "ui/ui.h"
#include "ui/mainmenu.h"

UI UI::ui;

msg_t UI::run(){
	MainMenu menu;
	button[0].clear_callbacks();
	button[1].clear_callbacks();
	button[2].clear_callbacks();
	
	button[0].set_press_handler((Button::button_handler)handle_right, (uint32_t*)this);
	button[1].set_press_handler((Button::button_handler)handle_select, (uint32_t*)this);
	button[2].set_press_handler((Button::button_handler)handle_left, (uint32_t*)this);
	menu.exec();
	return 0;
}

void UI::start(){
	if(thread){
		chThdTerminate(thread);
		chThdWait(thread);
	}
	thread = chThdCreateStatic(&UIThread, stack_size, NORMALPRIO - 1,
								(tfunc_t)start_thread, this);
}