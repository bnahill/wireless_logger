#include "ch.h"
#include "hal.h"

#include "platform.h"
#include "acquisition.h"


extern "C"{
	int main(void);
}

void ui_init();

int main(void) {
	uint32_t count;
	EventListener listener;
	
	halInit();
	chSysInit();
	ui_init();
	
	Acquisition::init();
	
	chEvtRegisterMask(&Acquisition::tick_source, &listener, 1);
	
	reg1.high_power(true);
	
	count = 0;
	while (TRUE) {
		chEvtWaitOne(1);
		if(++count == 50){
			count = 0;
			led1.toggle();
		}
	}
}

void ui_init(){
	
}