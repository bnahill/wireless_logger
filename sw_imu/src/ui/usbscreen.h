#ifndef __UI_USB_SCREEN_H_
#define __UI_USB_SCREEN_H_

#include "platform.h"
#include "acquisition.h"
#include "ui/ui.h"

//! @addtogroup UI
//! @{
//! @addtogroup Screens
//! @{

/*!
 @brief A screen where the USB functionality is enabled
 */
class USBScreen {
public:
	USBScreen(){}
	
	void exec();
protected:
	static constexpr uint32_t stack_size = 1024;
	
	WORKING_AREA(USBScreenThread, stack_size);
	
	msg_t terminate();
	
	Thread * thread;
	
	void thread_action();
	
	static msg_t start_thread(USBScreen * us){
		us->thread_action();
		return 0;
	}
};

//! @} @}

#endif // __UI_USB_SCREEN_H_
