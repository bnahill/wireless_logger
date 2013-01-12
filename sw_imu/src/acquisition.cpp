/*!
 @file platform.cpp
 
 @brief Threads realting to sensor acquisition
 
 @author Ben Nahill <bnahill@gmail.com>
 */

#include "acquisition.h"

EventSource Acquisition::tick_source;
EventSource Acquisition::tick_source2;
EventSource Acquisition::tick_source4;
EventSource Acquisition::tick_source8;

GPTDriver &Acquisition::timer = GPTD8;

static uint32_t mag_count = 0, acc_count = 0;

static WORKING_AREA(waAccThread, 512);
static msg_t AccThread(void *arg) {
	EventListener listener;
	(void)arg;
	chRegSetThreadName("AccThread");
	
	acc1.init();

	chEvtRegisterMask(&Acquisition::tick_source, &listener, 1);
	Euclidean3_f32 measurement;
	while(1){
		chEvtWaitOne(1);
		acc1.read();
		acc_count++;
		acc1.get_reading(measurement);
		if(measurement.mag_squared()> 3){
			//led1.on();
		} else {
			//led1.off();
		}
	}
}


static WORKING_AREA(waMagThread, 512);
static msg_t MagThread(void *arg) {
	EventListener listener;
	(void)arg;
	chRegSetThreadName("MagThread");

	chEvtRegisterMask(&Acquisition::tick_source4, &listener, 1);
	
	mag1.init();
	mag1.set_mode(MAG3110::MODE_ACTIVE);
	
	
	while(1){
		chEvtWaitOne(1);
		mag1.read();
		mag_count++;
	}
}

static WORKING_AREA(waGyroThread, 512);
static msg_t GyroThread(void *arg) {
	EventListener listener, listener8;
	eventmask_t event;
	(void)arg;
	chRegSetThreadName("GyroThread");

	chEvtRegisterMask(&Acquisition::tick_source, &listener, 1);
	chEvtRegisterMask(&Acquisition::tick_source8, &listener8, 8);
	
	gyro1.init();
	gyro1.set_full_scale(decltype(gyro1)::FS_250);
	gyro1.set_power_mode(decltype(gyro1)::POWER_NORMAL);
	
	while(1){
		event = chEvtWaitOne(8 + 1);
		switch(event){
			case 1:
				gyro1.read();
				break;
			case 8:
				gyro1.read_temperature();
				break;
			default:
				break;
		}
	}
}

void Acquisition::tick(GPTDriver* driver){
	static uint32_t count = 0;
	static uint32_t event;
	
	event = count ^ (count - 1);
	
	chEvtBroadcastFlags(&Acquisition::tick_source, 0);
	if(event & 2)
		chEvtBroadcastFlags(&Acquisition::tick_source2, 0);
	if(event & 4)
		chEvtBroadcastFlags(&Acquisition::tick_source4, 0);
	if(event & 8)
		chEvtBroadcastFlags(&Acquisition::tick_source8, 0);
	
	count = (count + 1) & 7;
}

void Acquisition::init(){
	chEvtInit(&Acquisition::tick_source);
	chEvtInit(&Acquisition::tick_source2);
	chEvtInit(&Acquisition::tick_source4);
	
	chEvtInit(&Acquisition::tick_source8);
	chThdCreateStatic(waAccThread, sizeof(waAccThread), NORMALPRIO, AccThread, NULL);
	chThdCreateStatic(waMagThread, sizeof(waMagThread), NORMALPRIO, MagThread, NULL);
	chThdCreateStatic(waGyroThread, sizeof(waGyroThread), NORMALPRIO, GyroThread, NULL);
	static GPTConfig const gpt_config = {10000, Acquisition::tick};
	
	gptStart(&timer, &gpt_config);
	gptStartContinuous(&timer, 100);
}