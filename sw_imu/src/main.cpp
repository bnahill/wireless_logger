#include "ch.h"
#include "hal.h"

#include "platform.h"
#include "acquisition.h"
#include "stdio.h"

extern "C"{
	int main(void);
}

void ui_init();

static enum {
	MODE_ACC,
	MODE_GYRO,
	MODE_MAG
} mode = MODE_ACC;

void handle_button1(uint32_t *){
	mode = MODE_ACC;
}

void handle_button2(uint32_t *){
	mode = MODE_GYRO;
}

void handle_button3(uint32_t *){
	mode = MODE_MAG;
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
	
	button[0].set_press_handler(handle_button1);
	button[1].set_press_handler(handle_button2);
	button[2].set_press_handler(handle_button3);
	/*
	oled.fb.draw_horizontal_mask(3, 0x80, 0, 127);
	oled.fb.draw_horizontal_mask(0, 0x88, 0, 25);
	oled.fb.draw_vertical(0, 3, 62);
	*/
	
	Euclidean3_f32 measurement;
	
	while (TRUE) {
		chEvtWaitOne(1);
		if(++count == 50){
			count = 0;
			led1.assign(reg1.is_charging());
		}

		oled.fb.clear_area(0, 3, 0, 90);
		switch(mode){
		case MODE_ACC:
			oled.fb.write_text<Courier3>("ACC:", 0, 0, 90);
			acc1.get_reading(measurement);
			break;
		case MODE_GYRO:
			oled.fb.write_text<Courier3>("GYR:", 0, 0, 90);
			gyro1.get_reading(measurement);
			break;
		case MODE_MAG:
			oled.fb.write_text<Courier3>("MAG:", 0, 0, 90);
			mag1.get_reading(measurement);
			break;
		}
		float_to_string(measurement.x, some_string);
		oled.fb.write_text<SmallFont>(some_string, 0, 90);
		float_to_string(measurement.y, some_string);
		oled.fb.write_text<SmallFont>(some_string, 1, 90);
		float_to_string(measurement.z, some_string);
		oled.fb.write_text<SmallFont>(some_string, 2, 90);
		
		oled.update();
	}
}

void ui_init(){
	Platform::EXTInit();
	button[0].enable();
	button[1].enable();
	button[2].enable();
}