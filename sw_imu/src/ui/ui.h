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
	typedef enum {
		MASK_ABORT   = 0x80000000,
		MASK_SUSPEND = 0x40000000,
		MASK_RESUME  = 0x20000000,

		MASK_SELECT  = 0x04000000,
		MASK_LEFT    = 0x02000000,
		MASK_RIGHT   = 0x01000000,
		MASK_BUTTONS = MASK_LEFT | MASK_SELECT | MASK_RIGHT,
	} ui_event_t;
	
	//! The main UI thread
	Thread * thread;
	//! A thread to just monitor the device and periodically update
	Thread * monitor_thread;
	
	VirtualTimer timer;
	
	/*!
	 @brief Handle any global events
	 @param evt New event mask
	 @return Mask still to handle
	 */
	eventmask_t handle_evt(eventmask_t evt);
	
	static void wait_for_button(ui_event_t evt){
		chEvtGetAndClearEvents(MASK_LEFT | MASK_RIGHT | MASK_SELECT);
		chEvtWaitAny(evt);
		chEvtGetAndClearEvents(MASK_LEFT | MASK_RIGHT | MASK_SELECT);
	}
	
	//! A singe static instance
	static UI ui;
	
	/*!
	 @brief A very simple function to kill the current UI operation
	 */
	static void exit_current(){
		chEvtSignal(ui.thread, UI::MASK_ABORT);	
	}
	
	/*!
	 @brief A launcher for screens defined as a class with an 'exec' method
	 @tparam screenclass The class name of the screen
	 */
	template<class screenclass>
	static void launch_screen(){
		screenclass s;
		s.exec();
	}
	
	void inhibit_suspend(){
		suspend_enabled = false;
		chVTReset(&timer);
	}
	
	void enable_suspend(){
		suspend_enabled = false;
		resume();
	}
	
	//! The thread stack size to use
	static constexpr uint32_t stack_size = 8192*2;
	
	static constexpr uint32_t monitor_stack_size = 1024;
	
	static constexpr uint32_t suspend_timeout_ms = 30000;
private:
	UI() :
		thread(nullptr), monitor_thread(nullptr), is_suspended(false),
		suspend_enabled(true)
	{}
	
	msg_t run();
	msg_t run_monitor();
	
	bool is_suspended;
	bool suspend_enabled;
	
	void resume(){
		if(suspend_enabled){
			start_suspend_timer();
		}
		if(is_suspended){
			is_suspended = false;
			Platform::oled.resume();
		}
	}
	
	void suspend(){
		Platform::oled.sleep();
		is_suspended = true;
	}
	
	
	void start_suspend_timer(){
		chSysLock();
		chVTReset(&timer);
		chVTSetI(&timer, MS2ST(suspend_timeout_ms), (vtfunc_t)handle_suspend, this);
		chSysUnlock();
	}
	
	WORKING_AREA(MonitorThread, monitor_stack_size);
	//! Stack area for the thread
	WORKING_AREA(UIThread, stack_size);

	
	static msg_t start_thread(UI * the_ui){
		return the_ui->run();
	}
	
	static msg_t start_monitor_thread(UI * the_ui){
		return the_ui->run_monitor();
	}
	
	static void handle_left(UI * the_ui){
		chEvtSignal(the_ui->thread, MASK_LEFT | MASK_RESUME);
	}
	
	static void handle_right(UI * the_ui){
		chEvtSignal(the_ui->thread, MASK_RIGHT | MASK_RESUME);
	}
	
	static void handle_select(UI * the_ui){
		chEvtSignal(the_ui->thread, MASK_SELECT | MASK_RESUME);
	}
	
	static void handle_suspend(UI * the_ui){
		chEvtSignal(the_ui->thread, MASK_SUSPEND);
	}
};

//! @}

#endif // __UI_H_
