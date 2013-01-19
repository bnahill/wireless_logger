#ifndef __IMU_LTC3559_H_
#define __IMU_LTC3559_H_

#include "ch.h"
#include "hal.h"

#include "imu/imu.h"

//! @addtogroup IMU
//! @{
//! @addtogroup LTC3559
//! @{

/*!
 @brief Linear Tech LTC3559 USB Li-ion charger with dual buck regulators
 */
class LTC3559 {
public:
	//! The mode for both buck regulators
	typedef enum {
		//! A low-noise mode capable of driving up to 400mA
		MODE_PULSE_SKIP,
		//! A more efficient mode only capable of driving about 100mA
		MODE_BURST
	} buck_mode_t;
	
	/*!
	 @brief Constructor
	 */
	LTC3559(gpio_pin_t const &susp_pin,
	        gpio_pin_t const &hp_pin,
	        gpio_pin_t const &mode_pin,
	        gpio_pin_t const &nchrg_pin) :
		susp_pin(susp_pin), hp_pin(hp_pin),
		mode_pin(mode_pin), nchrg_pin(nchrg_pin)
	{
		suspend(false);
		high_power(true);
		buck_mode(MODE_BURST);
	}
	
	//! Enable or disable USB suspend
	void suspend(bool new_susp) const;
	
	//! Enable or disable high-power (>100mA draw from USB)
	void high_power(bool new_hp) const;

	//! Set a new mode for the buck regulator
	void buck_mode(buck_mode_t new_mode) const;
	
	//! Indicate whether or battery is charging
	bool is_charging() const {return !nchrg_pin.read();}
protected:
	//! IO resources for control pins
	gpio_pin_t const susp_pin, hp_pin, mode_pin, nchrg_pin;
};

//! @} @}

#endif // __IMU_LTC3559_H_