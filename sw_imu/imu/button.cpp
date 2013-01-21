#include "button.h"

template <EXTDriver &driver>
Button< driver > *Button< driver >::buttons[num_channels];

template <EXTDriver &driver>
void Button< driver >::callback(EXTDriver* , expchannel_t channel){
	if(buttons[channel])
		buttons[channel]->handle_callback();
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

