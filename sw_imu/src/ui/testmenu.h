#ifndef __TESTMENU_H_
#define __TESTMENU_H_

#include "ui/ui.h"
#include "ui/menu.h"

//! @addtogroup UI
//! @{
//! @addtogroup Screens
//! @{

/*!
 @brief A menu containing a bunch of tests that can be executed
 */
class TestMenu {
public:
	TestMenu() : 
		menu(items, item_count), should_exit(false)
	{}
	
	void exec();
private:
	static void cb_close(uint32_t *){
		chEvtSignal(UI::ui.thread, UI::MASK_ABORT);
	}
	
	static void exit();
	
	static void launch_flash_test();
	
	Menu menu;
	
	bool should_exit;
	
	static const uint32_t item_count;
	static MenuItem const items[];
};

//! @} @}

#endif // __TESTMENU_H_
