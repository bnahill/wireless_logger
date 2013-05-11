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

/*!
 @brief Write an unsigned integer ('\0'-terminated) into a string
 @param i The integer
 @param s The string to start writing
 @param ignore_leading Do you want some arbitrary number of leading zeros?
 @return Pointer to new '\0' termination
 */
char * uint_to_string(uint32_t i, char * s, bool ignore_leading=true);

/*!
 @brief Write a float ('\0'-terminated) to a string
 @param f The float
 @param s Where to write it
 @return Pointer to new '\0' termination
 */
char * float_to_string(float f, char * s);

/*!
 @brief A wrapper for printing unknown objects
 
 If they have an imu_print method, everything will be okay. If you get compiler
 errors here, you are trying to print something that hasn't been described.
 */
template <typename T>
static inline char * imu_sprint_inner(char * dst, T v){
	return v.imu_print(dst);
}

template <>
char * imu_sprint_inner(char * dst, int i){
	return uint_to_string(static_cast<uint32_t>(i), dst);
}

template <>
char * imu_sprint_inner(char * dst, float f){
	return float_to_string(f, dst);
}

template <>
char * imu_sprint_inner(char * dst, const char * c){
    while(*c){
        *(dst++) = *(c++);
    }
    *dst = '\0';
    return dst;
}

template <>
char * imu_sprint_inner(char * dst, uint32_t u){
	return uint_to_string(u, dst);
}

template<typename Thead>
char * imu_sprint(char  * dst, const Thead& head){
    return imu_sprint_inner(dst, head);
}

/*!
 @brief Print function which doesn't need to allocate memory
 
 Using the new variadic templates from C++11, we can iterate through arguments
 safely, printing each one after the previous. All types must have a defined
 imu_sprint function in imu.h or must implement their own imu_print menthod.
 */
template<typename Thead, typename... Ttail> 
char * imu_sprint(char  * dst, const Thead& head, const Ttail&... tail){
    return imu_sprint(imu_sprint_inner(dst, head), tail...);
}


//! @} @}

#endif // __IMU_H_
