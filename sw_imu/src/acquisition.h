#ifndef __ACQUISITION_H_
#define __ACQUISITION_H_

#include "platform/platform.h"

namespace Acquisition {
	/*!
	 @brief Initialize acquisition system
	 */
	void init();
	
	//! Callback from ChibiOS timer driver
	void tick(GPTDriver *driver);
	
	//! Primary sample clock
	extern GPTDriver &timer;
	
	/*!
	 @name Tick sources
	 Tick sources as dividers of primary sample clock
	 @{
	 */
	extern EventSource tick_source;
	extern EventSource tick_source2;
	extern EventSource tick_source4;
	extern EventSource tick_source8;
	//! @}
};




#endif // __ACQUISITION_H_
