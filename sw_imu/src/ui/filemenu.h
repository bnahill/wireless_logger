#ifndef __FILEMENU_H_
#define __FILEMENU_H_

#include "ui/ui.h"
#include "ui/menu.h"

//! @addtogroup UI
//! @{
//! @addtogroup Screens
//! @{

/*!
 @brief A menu containing filesystem operations
 */
class FileMenu {
public:
	FileMenu() : 
		menu(items, item_count), should_exit(false)
	{}
	
	void exec();
private:
	static void cb_close(uint32_t *){
		chEvtSignal(UI::thread, UI::MASK_ABORT);
	}
	
	static void exit();
	
	static void launch_flash_format();
	static void launch_file_list();
	
	Menu menu;
	
	bool should_exit;
	
	static const uint32_t item_count;
	static MenuItem const items[];
};

//! @} @}

#endif // __FILEMENU_H_
