/*!
 @file imu.h
 @brief Declarations for common IMU functions and types
 @author Ben Nahill <bnahill@gmail.com>
 */

#ifndef __IMU_H_
#define __IMU_H_

//! @addtogroup IMU
//! @{
//! @addtogroup Util
//! @{

#ifndef BIT
#define BIT(x) (1 << x)
#endif

#include "imu/imu_math.h"

/*!
 @brief A structure defining a single GPIO pin
 */
struct gpio_pin_t {
	gpio_pin_t(GPIO_TypeDef *gpio, uint16_t index) :
		gpio(*gpio), index(index), mask(1 << index)
	{}
	
	/*!
	 @brief Read the current value at the pin
	 */
	bool read() const{
		return gpio.IDR & mask;
	}
	
	/*!
	 @brief Toggle the output of the pin
	 */
	void toggle() const{
		if(gpio.ODR & mask){
			gpio.BSRR.H.clear = mask;
		} else {
			gpio.BSRR.H.set = mask;
		}
	}
	
	/*!
	 @brief Assign a boolean value to the pin
	 */
	void assign(bool val) const{
		if(val)
			set();
		else
			clear();
	}
	
	/*!
	 @brief Explicitly set the output to a logical '1'
	 */
	void set() const{
		gpio.BSRR.H.set = mask;
	}
	
	/*!
	 @brief Explicitly set the output to a logical '0'
	 */
	void clear() const{
		gpio.BSRR.H.clear = mask;
	}
	GPIO_TypeDef &gpio;
	uint16_t mask;
	uint16_t index;
};

char *uint_to_string(uint32_t i, char * s, bool ignore_leading=true);

void float_to_string(float f, char * s);

//! @} @}

// A class which must be locked to use
// class Lockable {
// public:
// 	virtual void lock() = 0;
// 	virtual void unlock() = 0;
// 	virtual void reconfig_clock() = 0;
// };
// 
// class LockableSem : public Lockable {
// public:
// 	void lock(){
// 		chSemWait(&sem);
// 	}
// 	
// 	void unlock(){
// 		chSemSignal(&sem);
// 	}
// protected:
// 	LockableSem(){
// 		chSemInit(&sem, 1);
// 	}
// 	
// 	Semaphore sem;
// };

#endif // __IMU_H_
