#ifndef __UI_H_
#define __UI_H_

#include "platform.h"

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
	static void start();
	
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
	static Thread * thread;
	//! A thread to just monitor the device and periodically update
	static Thread * monitor_thread;
	
	static VirtualTimer timer;
	
	/*!
	 @brief Handle any global events
	 @param evt New event mask
	 @return Mask still to handle
	 */
	static eventmask_t handle_evt(eventmask_t evt);
	
	static void wait_for_button(ui_event_t evt, bool allow_sleep=true);
	
	/*!
	 @brief A very simple function to kill the current UI operation
	 */
	static void exit_current(){
		chEvtSignal(thread, UI::MASK_ABORT);	
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
	
	static void inhibit_suspend(){
		suspend_enabled = false;
		chVTReset(&timer);
	}
	
	static void enable_suspend(){
		suspend_enabled = true;
		resume();
	}
	
	//! The thread stack size to use
	static constexpr uint32_t stack_size = 8192*2;
	
	static constexpr uint32_t monitor_stack_size = 1024;
	
	static constexpr uint32_t suspend_timeout_ms = 30000;
private:
	static msg_t run(void * nothing);
	static msg_t run_monitor(void * nothing);
	
	static bool is_suspended;
	static bool suspend_enabled;
	
	static void resume(){
		if(suspend_enabled){
			start_suspend_timer();
		}
		if(is_suspended){
			is_suspended = false;
			Platform::oled.resume();
		}
	}
	
	static void suspend(){
		Platform::oled.sleep();
		is_suspended = true;
	}
	
	
	static void start_suspend_timer(){
		chSysLock();
		if(timer.vt_func)
			chVTResetI(&timer);
		chVTSetI(&timer, MS2ST(suspend_timeout_ms), (vtfunc_t)handle_suspend, nullptr);
		chSysUnlock();
	}
	
	static WORKING_AREA(MonitorThread, monitor_stack_size);
	//! Stack area for the thread
	static WORKING_AREA(UIThread, stack_size);
	
	static void handle_left(void * nothing){
		chSysLockFromIsr();
		chEvtSignalI(thread, MASK_LEFT | MASK_RESUME);
		chSysUnlockFromIsr();
	}
	
	static void handle_right(void * nothing){
		chSysLockFromIsr();
		chEvtSignalI(thread, MASK_RIGHT | MASK_RESUME);
		chSysUnlockFromIsr();
	}
	
	static void handle_select(void * nothing){
		chSysLockFromIsr();
		chEvtSignalI(thread, MASK_SELECT | MASK_RESUME);
		chSysUnlockFromIsr();
	}
	
	static void handle_suspend(void * nothing){
		chSysLockFromIsr();
		timer.vt_func = nullptr;
		chEvtSignalI(thread, MASK_SUSPEND);
		chSysUnlockFromIsr();
	}
};

//! @}

#endif // __UI_H_
