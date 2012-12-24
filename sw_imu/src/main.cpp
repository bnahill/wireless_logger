#include "ch.h"
#include "hal.h"

#include "platform.h"
#include "acquisition.h"


extern "C"{
	int main(void);
}

int main(void) {
	halInit();
	chSysInit();
	Acquisition::init();

	while (TRUE) {
		chThdSleepMilliseconds(TIME_INFINITE);
	}
}
