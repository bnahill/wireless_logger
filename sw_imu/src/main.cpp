#include "ch.h"
#include "hal.h"

#include "platform.h"
#include "acquisition.h"
#include <graphics/smallfont.h>

extern "C"{
	int main(void);
}

void ui_init();

void handle_button1(uint32_t *){
	oled.fb.write_text<SmallFont>("Apples",0,0);
	oled.fb.write_text<SmallFont>("are",1,0);
	oled.fb.write_text<SmallFont>("awesome!",2,0);
}

void handle_button2(uint32_t *){
	oled.fb.clear();
}

void handle_button3(uint32_t *){
	oled.fb.write_text<SmallFont>("EAT",1,64);
	oled.fb.write_text<SmallFont>("THEM",2,64);
	oled.fb.write_text<SmallFont>("ALWAYS!",3,64);
}

char *uint_to_string(uint32_t i, char * s, bool ignore_leading=true){
	uint32_t div;
	uint32_t rem;
	uint32_t place = 1000000;
	
	bool start_printing = !ignore_leading;
	
	for(place = 1000000; place; place /= 10){
		div = i / place;
		if(!div){
			if(start_printing || (place == 1))
				*(s++) = '0';
			continue;
		}
		*(s++) = div + '0';
		start_printing = true;
		i = i % place;
	}
	*s = 0;
	return s;
}

void float_to_string(float f, char * s){
	uint32_t frac = (int32_t)(f * (1 << 10));
	if(frac & 0x80000000){
		*(s++) = '-';
		frac = -frac;
	}
	s = uint_to_string(frac >> 10, s);
	f = frac & ((1 << 10) - 1);
	f *= 10000000.0 / ((float)0x3FF);
	*(s++) = '.';
	s = uint_to_string((uint32_t) f, s, false);
	*s = 0;
}

int main(void) {
	uint32_t count;
	EventListener listener;
	char some_string[64];
	char * tmp;
	
	halInit();
	chSysInit();
	ui_init();
	
	reg1.high_power(true);
	
	Acquisition::init();
	
	chEvtRegisterMask(&Acquisition::tick_source, &listener, 1);
	
	oled.init();
	
	count = 0;
	
	//button1.set_press_handler(handle_button1);
	//button2.set_press_handler(handle_button2);
	//button3.set_press_handler(handle_button3);
	/*
	oled.fb.draw_horizontal_mask(3, 0x80, 0, 127);
	oled.fb.draw_horizontal_mask(0, 0x88, 0, 25);
	oled.fb.draw_vertical(0, 3, 62);
	*/
	while (TRUE) {
		chEvtWaitOne(1);
		if(++count == 50){
			count = 0;
			
			led1.toggle();
		}
		if(count & 1){
			float_to_string(acc1.reading.x, some_string);
			oled.fb.write_text<SmallFont>(some_string, 0, 0);
			float_to_string(acc1.reading.y, some_string);
			oled.fb.write_text<SmallFont>(some_string, 1, 0);
			float_to_string(acc1.reading.z, some_string);
			oled.fb.write_text<SmallFont>(some_string, 2, 0);
			
			oled.update();
		}
	}
}

void ui_init(){
	Platform::EXTInit();
	button1.enable();
	button2.enable();
	button3.enable();
}