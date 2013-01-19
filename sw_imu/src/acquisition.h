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
	
	//! Sensor result source
	extern EventSource sensor_source;
	
	//! Set of flags to wait on for sensor readings
	typedef enum {
		SRC_ACC1  = 0x0001,
		SRC_ACC2  = 0x0002,
		SRC_ACC3  = 0x0004,
		SRC_ACC4  = 0x0008,
		SRC_MAG1  = 0x0010,
		SRC_MAG2  = 0x0020,
		SRC_MAG3  = 0x0040,
		SRC_MAG4  = 0x0080,
		SRC_GYRO1 = 0x0100,
		SRC_GYRO2 = 0x0200,
		SRC_GYRO3 = 0x0400,
		SRC_GYRO4 = 0x0800,
	} sensor_event_t;
};




#endif // __ACQUISITION_H_
