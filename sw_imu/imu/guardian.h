/*!
 @file guardian.h
 @brief Declarations for \ref Guardian and \ref GuardianRF classes
 @author Ben Nahill <bnahill@gmail.com>
 */

#ifndef __IMU_GUARDIAN_H_
#define __IMU_GUARDIAN_H_

#include "imu/cc1101.h"

//! @addtogroup IMU
//! @{
//! @addtogroup CGM
//! @{

/*!
 @brief The interface guardian GCM sensor
 @tparam iface_t The interface type to use. At the moment this is just
 the CC1101 but other chips may be useful later
 */
template< class iface_t >
class Guardian {
public:
	Guardian<iface_t>(iface_t &iface) :
		iface(iface)
	{
		
	}
	
	bool init();
	
	void sleep();
	
	void wakeup();
protected:
	iface_t &iface;
};

/*!
 @brief The wireless interface guardian GCM sensor using TI CC1101 RF chip
 */
class GuardianRF : public Guardian<CC1101> {
public:
	GuardianRF(CC1101 &iface, gpio_pin_t * gdo0 = nullptr,
	           gpio_pin_t * gdo2 = nullptr);
protected:
	gpio_pin_t * gdo0, * gdo2;
};

//! @} @}

#include "guardian.cpp"

#endif
