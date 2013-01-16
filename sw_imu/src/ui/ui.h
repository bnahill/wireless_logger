#ifndef __UI_H_
#define __UI_H_

#include "platform/platform.h"

//! @addtogroup UI
//! @{

/*!
 @brief An encapsulation for the entire GUI
 
 This creates its own thread and runs the GUI. Unless explicitly ignored,
 all button events are handled through here and processes spawned from here
 are expected to have sole control of the display.
 */
class UI {
public:
	//! Start the UI thread
	void start();
	
	/*!
	 @brief Mask for common UI events to handle
	 */
	enum {
		MASK_ABORT   = 0x80000000,
		MASK_SELECT  = 0x40000000,
		MASK_LEFT    = 0x20000000,
		MASK_RIGHT   = 0x10000000,
	} ui_event_t;
	
	//! The main UI thread
	Thread * thread;
	
	//! A singe static instance
	static UI ui;
	
	//! The thread stack size to use
	static constexpr uint32_t stack_size = 8192;
private:
	UI() :
		thread(nullptr)
	{}
	
	msg_t run();
	
	//! Stack area for the thread
	WORKING_AREA(UIThread, stack_size);
	
	static msg_t start_thread(UI * the_ui){
		return the_ui->run();
	}
	
	static void handle_left(UI * the_ui){
		chEvtSignal(the_ui->thread, MASK_LEFT);
	}
	
	static void handle_right(UI * the_ui){
		chEvtSignal(the_ui->thread, MASK_RIGHT);
	}
	
	static void handle_select(UI * the_ui){
		chEvtSignal(the_ui->thread, MASK_SELECT);
	}
};

//! @}

#endif // __UI_H_
