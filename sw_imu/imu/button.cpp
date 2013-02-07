#include "button.h"

template <EXTDriver &driver>
void Button< driver >::static_cb(void * arg){
	((Button< driver > *)arg)->handle_callback();
}

template <EXTDriver &driver>
void Button< driver >::handle_callback(){
	state_t new_state = check_gpio();
	// Transition!
	if(state != new_state){
		state = new_state;
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
}

