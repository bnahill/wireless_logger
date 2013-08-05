#ifndef __EVENTMENU_H_
#define __EVENTMENU_H_

#include "ui/ui.h"
#include "ui/menu.h"

//! @addtogroup UI
//! @{
//! @addtogroup Screens
//! @{



/*!
 @brief A screen to show a log of events
 */
class EventMenu {
public:
	EventMenu() : 
		should_exit(false)
	{}
	
	void exec();
private:
	static void cb_close(uint32_t *){
		chEvtSignal(UI::thread, UI::MASK_ABORT);
	}
	
	static void exit();
	
	static void launch_flash_format();
	
	bool should_exit;
	
	static const uint32_t item_count;
	static MenuItem const items[];
};

//! @} @}

#endif // __EVENTMENU_H_
