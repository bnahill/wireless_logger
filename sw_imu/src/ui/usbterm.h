#ifndef __USB_TERM_H_
#define __USB_TERM_H_

#include "platform.h"
#include "acquisition.h"
#include "ui/ui.h"

class USBTerm;

//! @addtogroup UI
//! @{
//! @addtogroup USB
//! @{
	
/*!
 @brief A simple class for defining shell commands by a root command and
 callback
 */
class ShellCommand{
public:
	//! Shell command callback type
	typedef int32_t (USBTerm::*shell_callback_t)(uint8_t const * cmd);
	
	ShellCommand(char const * root, char const * args, shell_callback_t cb) :
		root((uint8_t const *)root), args((uint8_t const *)args), cb(cb)
	{}
	
	int32_t call(uint8_t const * cmd, USBTerm &usb) const {
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
	
	uint8_t const * get_root() const {return root;}
	uint8_t const * get_args() const {return args;}
private:
	uint8_t const * root;
	uint8_t const * args;
	shell_callback_t const cb;
};

class USBTerm {
public:
	USBTerm(){}
	
	void launch(){
		thread = chThdCreateStatic(USBTermThread, stack_size, LOWPRIO,
	                           (tfunc_t) start_thread, (void *)this);	
	}
	
	//! Terminate the thread and wait for it to return
	msg_t terminate();
	
protected:
	static constexpr uint32_t stack_size = 1024;
	
	WORKING_AREA(USBTermThread, stack_size);
	
	Thread * thread;
	
	//! The thread function to be run
	void thread_action();
	
	
	//! A list of available commands
	static ShellCommand const commands[];
	//! The length of commands
	static uint32_t const num_commands;
	
	static msg_t start_thread(USBTerm * ut){
		ut->thread_action();
		return 0;
	}
	
	//! @name Commands
	//! @{
	int32_t cmd_help(char const *cmd);
	int32_t cmd_settime(char const *cmd);
	int32_t cmd_ping(char const *cmd);
	int32_t cmd_listcmds(char const *cmd);
	//! @}
};


//! @} @}

#endif // __USB_TERM_H_
