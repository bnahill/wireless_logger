#ifndef __MAINMENU_H_
#define __MAINMENU_H_

#include "ui/ui.h"
#include "ui/menu.h"

class MainMenu {
public:
	MainMenu() : 
		menu(items, item_count), should_exit(false)
	{}
	
	void exec();
private:
	static void cb_close(uint32_t *){
		chEvtSignal(UI::ui.thread, UI::MASK_ABORT);
	}
	
	static void launch_sensor_view_bar();
	static void launch_sensor_view_num();
	
	Menu menu;
	
	bool should_exit;
	
	static constexpr uint32_t item_count = 3;
	static MenuItem const items[item_count];
};

#endif // __MAINMENU_H_
