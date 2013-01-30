#ifndef __UI_USB_SCREEN_H_
#define __UI_USB_SCREEN_H_

#include "platform.h"
#include "acquisition.h"
#include "ui/ui.h"
#include "ui/usbterm.h"

//! @addtogroup UI
//! @{
//! @addtogroup Screens
//! @{

class USBScreen;


/*!
 @brief A screen where the USB functionality is enabled
 */
class USBScreen {
public:
	USBScreen(){}
	
	void exec();
protected:
	USBTerm term;
	msg_t terminate();
};

//! @} @}

#endif // __UI_USB_SCREEN_H_
