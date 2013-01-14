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
	
	halInit();
	chSysInit();
	ui_init();
	
	reg1.high_power(true);
	
	Acquisition::init();
	
	chEvtRegisterMask(&Acquisition::tick_source, &listener, 1);
	
	oled.init();
	
	enum {DISP_NUMBER, DISP_PROGRESS} display_mode;
	
	if(button[0].check_gpio() == Button::ST_PRESSED)
		display_mode = DISP_PROGRESS;
	else
		display_mode = DISP_NUMBER;
	
	count = 0;
	
	button[0].set_press_handler(handle_button1);
	button[1].set_press_handler(handle_button2);
	button[2].set_press_handler(handle_button3);
	
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
			if(display_mode == DISP_PROGRESS) measurement *= 0.2;
			break;
		case MODE_GYRO:
			oled.fb.write_text<Courier3>("GYR:", 0, 0, 90);
			gyro1.get_reading(measurement);
			if(display_mode == DISP_PROGRESS) measurement *= 0.01;
			break;
		case MODE_MAG:
			oled.fb.write_text<Courier3>("MAG:", 0, 0, 90);
			mag1.get_reading(measurement);
			if(display_mode == DISP_PROGRESS) measurement *= 0.01;
			break;
		}
		switch(display_mode){
		case DISP_NUMBER:
			float_to_string(measurement.x, some_string);
			oled.fb.write_text<SmallFont>(some_string, 0, 90);
			float_to_string(measurement.y, some_string);
			oled.fb.write_text<SmallFont>(some_string, 1, 90);
			float_to_string(measurement.z, some_string);
			oled.fb.write_text<SmallFont>(some_string, 2, 90);
			break;
		case DISP_PROGRESS:
			oled.fb.draw_pos_neg_bar(measurement.x, 0, 80, 127);
			oled.fb.draw_pos_neg_bar(measurement.y, 1, 80, 127);
			oled.fb.draw_pos_neg_bar(measurement.z, 2, 80, 127);
			break;
		}
		oled.update();
	}
}

void ui_init(){
	Platform::EXTInit();
	button[0].enable();
	button[1].enable();
	button[2].enable();
}