/*!
 @file ltc3559.cpp
 @brief Body for \ref LTC3559 class
 @author Ben Nahill <bnahill@gmail.com>
 */

#include "imu/ltc3559.h"

void LTC3559::buck_mode(buck_mode_t new_mode) const{
	switch(new_mode){
		case MODE_BURST:
			mode_pin.set();
			break;
		case MODE_PULSE_SKIP:
			mode_pin.clear();
			break;
	}
}

void LTC3559::high_power(bool new_hp) const {
	hp_pin.assign(new_hp);
}

void LTC3559::suspend(bool new_susp) const {
	susp_pin.assign(new_susp);
}
