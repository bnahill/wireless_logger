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
	
	while(1){
		chEvtWaitOne(1);
		acc1.read();
		acc_count++;
	}
}

static WORKING_AREA(waMagThread, 512);
static msg_t MagThread(void *arg) {
	EventListener listener;
	(void)arg;
	chRegSetThreadName("MagThread");

	chEvtRegisterMask(&Acquisition::tick_source4, &listener, 1);
	
	while(1){
		chEvtWaitOne(1);
		mag_count++;
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
	static GPTConfig const gpt_config = {10000, Acquisition::tick};
	
	gptStart(&timer, &gpt_config);
	gptStartContinuous(&timer, 200);
}