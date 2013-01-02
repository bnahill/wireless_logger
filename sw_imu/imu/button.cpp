#include "button.h"

Button *Button::buttons[num_channels];

void Button::callback(EXTDriver* , expchannel_t channel){
	if(buttons[channel])
		buttons[channel]->handle_callback();
}

void Button::handle_callback(){
	state_t new_state = check_gpio();
	// Transition!
	if(state != new_state){
		state = new_state;
		switch(new_state){
		case ST_PRESSED:
			if(press_handler){
				press_handler(param);
			}
		case ST_RELEASED:
			if(release_handler){
				release_handler(param);
			}
		}
	}
}

