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


EventSource Acquisition::sensor_source;


GPTDriver &Acquisition::timer = GPTD8;

static uint32_t acc_sample_count = 0;

using namespace Platform;

static WORKING_AREA(waAccThread, 512);
static msg_t AccThread(void *arg) {
	EventListener listener;
	Euclidean3_f32 reading;
	(void)arg;
	chRegSetThreadName("AccThread");
	
	acc1.set_dr(MMA8452Q::DR_100, false);
	acc1.init();

	chEvtRegisterMask(&Acquisition::tick_source, &listener, 1);
	while(1){
		chEvtWaitOne(1);
		acc1.read();
		acc1.get_reading(reading);
		acc_source.put(reading);
		acc_sample_count += 1;
		//chEvtBroadcastFlags(&Acquisition::sensor_source,
		//                    Acquisition::SRC_ACC1);
	}
	return 0;
}


static WORKING_AREA(waMagThread, 512);
static msg_t MagThread(void *arg) {
	EventListener listener;
	Euclidean3_f32 reading;
	(void)arg;
	chRegSetThreadName("MagThread");

	chEvtRegisterMask(&Acquisition::tick_source4, &listener, 1);
	
	mag1.init();
	mag1.set_mode(MAG3110::MODE_ACTIVE);
	
	
	while(1){
		chEvtWaitOne(1);
		mag1.read();
		mag1.get_reading(reading);
		mag_source.put(reading);
		//chEvtBroadcastFlags(&Acquisition::sensor_source,
		//                    Acquisition::SRC_MAG1);
	}
	return 0;
}

static WORKING_AREA(waGyroThread, 512);
static msg_t GyroThread(void *arg) {
	EventListener listener;
	eventmask_t event;
	Euclidean3_f32 reading;
	
	(void)arg;
	chRegSetThreadName("GyroThread");

	chEvtRegisterMask(&Acquisition::tick_source, &listener, 1);
	
	gyro1.init();
	gyro1.set_full_scale(decltype(gyro1)::FS_250);
	gyro1.set_power_mode(decltype(gyro1)::POWER_NORMAL);
	
	while(1){
		event = chEvtWaitOne(8 + 1);
		switch(event){
			case 1:
				gyro1.read();
				//chEvtBroadcastFlags(&Acquisition::sensor_source,
				//                    Acquisition::SRC_GYRO1);
				gyro1.get_reading(reading);
				gyro_source.put(reading);
				break;
			case 8:
				gyro1.read_temperature();
				break;
			default:
				break;
		}
	}
	return 0;
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
	
	chEvtInit(&Acquisition::sensor_source);

	chThdCreateStatic(waAccThread, sizeof(waAccThread), NORMALPRIO + 10, AccThread, NULL);
	chThdCreateStatic(waMagThread, sizeof(waMagThread), NORMALPRIO + 10, MagThread, NULL);
	chThdCreateStatic(waGyroThread, sizeof(waGyroThread), NORMALPRIO + 10, GyroThread, NULL);
	static GPTConfig const gpt_config = {10000, Acquisition::tick};
	
	gptStart(&timer, &gpt_config);
	gptStartContinuous(&timer, 100);
}