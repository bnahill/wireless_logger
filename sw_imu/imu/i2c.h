/*!
 @file i2c.h
 @brief Declaration for \ref I2C class
 @author Ben Nahill <bnahill@gmail.com>
 */

#ifndef __IMU_I2C_H_
#define __IMU_I2C_H_

#include "ch.h"
#include "hal.h"
#include "imu.h"

//! @addtogroup IMU
//! @{
//! @addtogroup I2C
//! @{

//class I2C : public Lockable {
class I2C {
public:
	/*!
	 @brief Constructor for I2C driver. Statically initialize with your
	        settings.
	 @param driver The ChibiOS I2C HAL driver
	 @param opmode The I2C mode for the ChibiOS driver
	 @param dutycycle The clock duty cycle to use
	 @param speed The clock rate in Hz
	 @param sda The GPIO pin for SDA to be used in resets of I2C
	 @param scl The GPIO pin for SCL to be used in resets of I2C
	 */
	I2C(I2CDriver &driver, i2copmode_t opmode, i2cdutycycle_t dutycycle,
	    uint32_t speed, gpio_pin_t const &sda, gpio_pin_t const &scl);
	
	/*!
	 @brief Start the driver. If it is already started it will merely count a
	 new reference.
	 */
	void init();
	
	/*!
	 @brief Close the driver. It will only close once all references have
	 closed.
	 */
	void close();
	
	/*!
	 @brief Lock the mutex for the driver
	 */
	void lock(){i2cAcquireBus(&driver);}
	
	/*!
	 @brief Release the mutex for the driver
	 */
	void unlock(){i2cReleaseBus(&driver);}
	
	void reconfig_clock();
	
	/*!
	 @brief Get the errors that were encountered
	 @return I2C Error flags
	 */
	i2cflags_t get_errors(){return i2cGetErrors(&driver);}
	
	msg_t transmit(i2caddr_t addr, const uint8_t *txbuf, size_t txbytes,
	               uint8_t *rxbuf, size_t rxbytes, systime_t timeout);
	
	msg_t receive(i2caddr_t addr, uint8_t * rxbuf, size_t rxbytes,
	              systime_t timeout);

	uint8_t read_byte_test(i2caddr_t addr, uint8_t regaddr);
	
	uint8_t read_byte(i2caddr_t addr, uint8_t regaddr);
	
	msg_t write_byte(i2caddr_t addr, uint8_t regaddr, uint8_t value);
	
protected:
	//! Driver configuration
	I2CConfig const config;
	//! Count of tasks dependent on this driver
	uint32_t refcnt;
	//! ChibiOS I2C driver
	I2CDriver &driver;
	
	gpio_pin_t const &sda;
	gpio_pin_t const &scl;
};

//! @} @}

#endif
