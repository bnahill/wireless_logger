/*!
 @file platform.cpp
 
 @brief Threads realting to sensor acquisition
 
 @author Ben Nahill <bnahill@gmail.com>
 */

#include "acquisition.h"

using namespace Platform;

EventSource Acquisition::tick_source;
EventSource Acquisition::tick_source2;
EventSource Acquisition::tick_source4;
EventSource Acquisition::tick_source8;


EventSource Acquisition::sensor_source;

VirtualTimer Acquisition::vtimer;

WORKING_AREA(Acquisition::waAccThread, 512);
WORKING_AREA(Acquisition::waGyroThread, 512);
WORKING_AREA(Acquisition::waMagThread, 512);

bool Acquisition::acc_enabled;
bool Acquisition::gyro_enabled;
bool Acquisition::mag_enabled;

msg_t Acquisition::AccThread(void *arg) {
	EventListener listener;
	Euclidean3_f32 reading;
	(void)arg;
	chRegSetThreadName("AccThread");
	
	chEvtRegisterMask(&Acquisition::tick_source, &listener, 1);
	
	acc1.set_dr(MMA8452Q::DR_100, false);
	acc1.init();
	
	acc1.set_active_mode(MMA8452Q::ACT_MODE_STANDBY);
	while(TRIG_WAKE != chEvtWaitOne(TRIG_ALL));
	acc1.set_active_mode(MMA8452Q::ACT_MODE_STANDBY);
	acc_enabled = true;

	while(1){
		switch(chEvtWaitOne(TRIG_ALL + 1)){
			case 1:
				acc1.read();
				acc1.get_reading(reading);
				acc_source.put(reading);
			case 8:
				gyro1.read_temperature();
				break;
			case TRIG_SLEEP:
				acc1.set_active_mode(MMA8452Q::ACT_MODE_STANDBY);
				acc_enabled = false;
				while(TRIG_WAKE !=
				      chEvtWaitOne(TRIG_ALL));
				acc1.set_active_mode(MMA8452Q::ACT_MODE_ACTIVE);
				acc_enabled = true;
				break;
			case TRIG_WAKE:
				// Don't do anything.
			default:
				break;
		}
	}
	return 0;
}



msg_t Acquisition::MagThread(void *arg) {
	EventListener listener;
	Euclidean3_f32 reading;
	(void)arg;
	chRegSetThreadName("MagThread");

	chEvtRegisterMask(&tick_source4, &listener, 1);
	
	mag1.init();
	
	mag1.set_mode(MAG3110::MODE_STANDBY);
	
	while(TRIG_WAKE != chEvtWaitOne(TRIG_ALL));
	
	mag1.set_mode(MAG3110::MODE_ACTIVE);
	mag_enabled = true;
	
	
	while(1){
		switch(chEvtWaitOne(TRIG_ALL + 1)){
		case 1:
			mag1.read();
			mag1.get_reading(reading);
			mag_source.put(reading);
			break;
		case TRIG_SLEEP:
			mag1.set_mode(MAG3110::MODE_STANDBY);
			mag_enabled = false;
			while(TRIG_WAKE !=
			      chEvtWaitOne(TRIG_ALL));
			mag1.set_mode(MAG3110::MODE_ACTIVE);
			mag_enabled =true;
			break;
		case TRIG_WAKE:
			// Don't do anything.
		default:
			break;
		}
	}
	return 0;
}


msg_t Acquisition::GyroThread(void *arg) {
	EventListener listener;
	Euclidean3_f32 reading;
	(void)arg;
	chRegSetThreadName("GyroThread");

	chEvtRegisterMask(&tick_source, &listener, 1);
	
	gyro1.init();
	
	gyro1.set_full_scale(decltype(gyro1)::FS_250);
	
	gyro1.set_power_mode(decltype(gyro1)::POWER_DOWN);
	while(TRIG_WAKE != chEvtWaitOne(TRIG_ALL));
	
	gyro1.set_power_mode(decltype(gyro1)::POWER_NORMAL);
	gyro_enabled = true;
	
	while(1){
		switch(chEvtWaitOne(TRIG_ALL + 8 + 1)){
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
			case TRIG_SLEEP:
				gyro1.set_power_mode(decltype(gyro1)::POWER_DOWN);
				gyro_enabled = false;
				while(TRIG_WAKE !=
				      chEvtWaitOne(TRIG_ALL));
				gyro1.set_power_mode(decltype(gyro1)::POWER_NORMAL);
				gyro_enabled = true;
				break;
			case TRIG_WAKE:
				// Don't do anything.
			default:
				break;
		}
	}
	return 0;
}

void Acquisition::tick(void * nothing){
	static uint32_t count = 0;
	static uint32_t event;
	
	chSysLockFromIsr();
	
	chVTSetI(&vtimer, MS2ST(10), Acquisition::tick, nullptr);
	
	event = count ^ (count - 1);
	
	chEvtBroadcastFlagsI(&Acquisition::tick_source, 0);
	if(event & 2)
		chEvtBroadcastFlagsI(&Acquisition::tick_source2, 0);
	if(event & 4)
		chEvtBroadcastFlagsI(&Acquisition::tick_source4, 0);
	if(event & 8)
		chEvtBroadcastFlagsI(&Acquisition::tick_source8, 0);
	
	count = (count + 1) & 7;
	chSysUnlockFromIsr();
}

Thread * Acquisition::acc_thread;
Thread * Acquisition::gyro_thread;
Thread * Acquisition::mag_thread;

void Acquisition::init(){
	chEvtInit(&Acquisition::tick_source);
	chEvtInit(&Acquisition::tick_source2);
	chEvtInit(&Acquisition::tick_source4);
	chEvtInit(&Acquisition::tick_source8);
	
	chEvtInit(&Acquisition::sensor_source);
	
	acc_enabled = false;
	gyro_enabled = false;
	mag_enabled = false;
	

	acc_thread = chThdCreateStatic(waAccThread, sizeof(waAccThread), NORMALPRIO + 10, AccThread, NULL);
	mag_thread = chThdCreateStatic(waMagThread, sizeof(waMagThread), NORMALPRIO + 10, MagThread, NULL);
	gyro_thread = chThdCreateStatic(waGyroThread, sizeof(waGyroThread), NORMALPRIO + 10, GyroThread, NULL);
	
	//static GPTConfig const gpt_config = {10000, Acquisition::tick};
	
	//gptStart(&timer, &gpt_config);
	//gptStartContinuous(&timer, 100);
	chSysLock();
	chVTSetI(&vtimer, MS2ST(10), Acquisition::tick, nullptr);
	chSysUnlock();
}

void Acquisition::enable_sources(Acquisition::sensor_src_t sensor){
	if(sensor & SRC_ACC1){
		chEvtSignal(acc_thread, TRIG_WAKE);
	}
	if(sensor & SRC_GYRO1){
		chEvtSignal(gyro_thread, TRIG_WAKE);
	}
	if(sensor & SRC_MAG1){
		chEvtSignal(mag_thread, TRIG_WAKE);
	}
}

void Acquisition::disable_sources(Acquisition::sensor_src_t sensor){
	if(sensor & SRC_ACC1){
		chEvtSignal(acc_thread, TRIG_SLEEP);
	}
	if(sensor & SRC_GYRO1){
		chEvtSignal(gyro_thread, TRIG_SLEEP);
	}
	if(sensor & SRC_MAG1){
		chEvtSignal(mag_thread, TRIG_SLEEP);
	}
}
