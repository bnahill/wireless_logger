#ifndef __IMU_GUARDIAN_H_
#define __IMU_GUARDIAN_H_

#include "imu/cc1101.h"

template< class iface_t >
class Guardian {
public:
	Guardian<iface_t>(iface_t &iface) :
		iface(iface)
	{
		
	}
	
	bool init(){
		if(!iface.init())
			return false;
		return true;
	}
	
	void sleep();
	
	void wakeup();
protected:
	iface_t &iface;
};

class GuardianRF : public Guardian<CC1101> {
public:
	GuardianRF(CC1101 &iface, gpio_pin_t * gdo0 = nullptr,
	           gpio_pin_t * gdo2 = nullptr);
protected:
	gpio_pin_t * gdo0, * gdo2;
};

#include "guardian.cpp"

#endif
