#ifndef __ACQUISITION_H_
#define __ACQUISITION_H_

#include "platform/platform.h"

namespace Acquisition {
	void init();
	void tick(GPTDriver *driver);
	
	extern GPTDriver &timer;
	
	extern EventSource tick_source;
	extern EventSource tick_source2;
	extern EventSource tick_source4;
	extern EventSource tick_source8;
};




#endif // __ACQUISITION_H_
