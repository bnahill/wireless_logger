#ifndef __UI_H_
#define __UI_H_

#include "platform/platform.h"

class UI;





class UI {
public:
	UI() :
		thread(nullptr)
	{
	}
	
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
	
	Thread * thread;
	
	static UI ui;
private:
	msg_t run();
	
	static msg_t start_thread(UI * the_ui){
		return the_ui->run();
	}
	
	static constexpr uint32_t stack_size = 8192;
	
	static void handle_left(UI * the_ui){
		chEvtSignal(the_ui->thread, MASK_LEFT);
	}
	
	static void handle_right(UI * the_ui){
		chEvtSignal(the_ui->thread, MASK_RIGHT);
	}
	
	static void handle_select(UI * the_ui){
		chEvtSignal(the_ui->thread, MASK_SELECT);
	}
	
	WORKING_AREA(UIThread, stack_size);
};

#endif // __UI_H_
