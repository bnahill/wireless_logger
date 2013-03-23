/*!
 @file button.cpp
 @brief Body for \ref Button class
 @author Ben Nahill <bnahill@gmail.com>
 */

#include "button.h"

template <EXTDriver &driver>
void Button< driver >::static_cb(Button< driver > * arg){
	arg->handle_callback();
}

template <EXTDriver &driver>
void Button< driver >::handle_callback(){
	state_t new_state = check_gpio();
	// Transition!
	if(state != new_state){
		state = new_state;
		extChannelDisable(&driver, channel);
		chVTSetI(&timer, MS2ST(debounce_ms),
		         reinterpret_cast<vtfunc_t>(debounce_cb), this);
	}
}

template <EXTDriver &driver>
void Button< driver >::debounce_cb ( Button< driver > * arg ) {
	arg->handle_debounce();
}

template <EXTDriver &driver>
void Button< driver >::handle_debounce(){
	state_t new_state = check_gpio();
	// Check if debounce seems okay
	if(state == new_state){
		switch(new_state){
		case ST_PRESSED:
			if(press_handler){
				press_handler(press_param);
			}
		case ST_RELEASED:
			if(release_handler){
				release_handler(release_param);
			}
		}
	}
	state = new_state;
	extChannelEnable(&driver, channel);
}