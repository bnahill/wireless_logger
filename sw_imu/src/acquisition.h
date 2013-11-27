#ifndef __ACQUISITION_H_
#define __ACQUISITION_H_

#include "platform.h"

//! @addtogroup IMU
//! @{
//! @addtogroup DataAcquisition
//! @{

/*!
 @brief The high-level interface for sensor data acquisition

 This is a static class which allows for individual sensors to be accessed
 safely by all tasks in the system.
 */
class Acquisition {
public:
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
	} sensor_src_t;
	
	/*!
	 @brief Initialize acquisition system
	 */
	static void init();
	
	static bool acc_is_enabled(){return acc_enabled;}
	static bool gyro_is_enabled(){return gyro_enabled;}
	static bool mag_is_enabled(){return mag_enabled;}
	
	
	/*!
	 @brief Increment reference count on sources, enable if not already
	 @param sensor_mask Mask of OR of sensor_src_t sensors
	 */
	static void require_sources(uint32_t sensor_mask);
	/*!
	 @brief Decrement reference count on sources, disable if no others
	 @param sensor_mask Mask of OR of sensor_src_t sensors
	 */
	static void norequire_sources(uint32_t sensor_mask);
private:
	//! Callback from ChibiOS timer driver
	static void tick(void * nothing);

	//! Primary sample clock, based on OS tick
	static VirtualTimer vtimer;
	
	static bool acc_enabled, gyro_enabled, mag_enabled;
	
	/*!
	 @name Tick sources
	 Tick sources as dividers of primary sample clock
	 @{
	 */
	static EventSource tick_source;
	static EventSource tick_source2;
	static EventSource tick_source4;
	static EventSource tick_source8;
	//! @}
	
	/*!
	 @name Sensor reference counts
	 A count of the number of tasks that indicate that they require each sensor
	 @{
	 */
	static uint32_t acc_ref_count, mag_ref_count, gyro_ref_count;
	//! @}
	
	//! Sensor result source
	static EventSource sensor_source;
	
	typedef enum {
		TRIG_SLEEP = 0x10000,
		TRIG_WAKE = 0x20000,
		TRIG_ALL = 0xF0000
	} trigger_event_t;
	
	static Thread * acc_thread, * gyro_thread, * mag_thread;
	
	static msg_t AccThread(void *arg);
	static msg_t GyroThread(void *arg);
	static msg_t MagThread(void *arg);

	static constexpr uint32_t acq_thread_stack_size = 128;
	
	static WORKING_AREA(waAccThread, acq_thread_stack_size);
	static WORKING_AREA(waGyroThread, acq_thread_stack_size);
	static WORKING_AREA(waMagThread, acq_thread_stack_size);
};

//! @} @}


#endif // __ACQUISITION_H_
