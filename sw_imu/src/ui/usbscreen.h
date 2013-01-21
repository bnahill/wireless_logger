#ifndef __UI_USB_SCREEN_H_
#define __UI_USB_SCREEN_H_

#include "platform.h"
#include "acquisition.h"
#include "ui/ui.h"

//! @addtogroup UI
//! @{
//! @addtogroup Screens
//! @{

class USBScreen;

/*!
 @brief A simple class for defining shell commands by a root command and
 callback
 */
class ShellCommand{
public:
	//! Shell command callback type
	typedef int32_t (USBScreen::*shell_callback_t)(uint8_t const * cmd);
	
	ShellCommand(char const * root, shell_callback_t cb) :
		root((uint8_t const *)root), cb(cb)
	{}
	
	int32_t call(uint8_t const * cmd, USBScreen &usb) const {
		return (usb.*cb)(cmd);
	}
	
	bool match(uint8_t const * cmd) const {
		uint8_t const * iter1;
		uint8_t const * iter2;
		iter1 = root;
		iter2 = cmd;
		while(*iter1 == *iter2){
			iter1 += 1;
			iter2 += 1;
		}
		if((0 == *iter1) && ((*iter2 == ' ') || (*iter2 == '\r')))
			return true;
		return false;
	}
private:
	uint8_t const * root;
	shell_callback_t const cb;
};


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
	
	//! Terminate the thread and wait for it to return
	msg_t terminate();
	
	Thread * thread;
	
	//! The thread function to be run
	void thread_action();
	
	//! A list of available commands
	static ShellCommand const commands[];
	//! The length of commands
	static uint32_t const num_commands;
	
	//! @name Commands
	//! @{
	int32_t cmd_help(char const *cmd);
	int32_t cmd_settime(char const *cmd);
	//! @}
	
	static msg_t start_thread(USBScreen * us){
		us->thread_action();
		return 0;
	}
};

//! @} @}

#endif // __UI_USB_SCREEN_H_
