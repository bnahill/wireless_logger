#ifndef __IMU_I2C_H_
#define __IMU_I2C_H_

#include "ch.h"
#include "hal.h"

//! @addtogroup IMU
//! @{
//! @addtogroup I2C
//! @{

class I2C {
public:
	/*!
	 @brief Constructor for I2C driver. Statically initialize with your
	        settings.
	 @param driver The ChibiOS I2C HAL driver
	 @param opmode The I2C mode for the ChibiOS driver
	 @param dutycycle The clock duty cycle to use
	 @param speed The clock rate in Hz
	 */
	I2C(I2CDriver &driver, i2copmode_t opmode, i2cdutycycle_t dutycycle,
	    uint32_t speed);
	
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
	void acquire(){i2cAcquireBus(&driver);}
	
	/*!
	 @brief Release the mutex for the driver
	 */
	void release(){i2cReleaseBus(&driver);}
	
	/*!
	 @brief Get the errors that were encountered
	 @return I2C Error flags
	 */
	i2cflags_t get_errors(){return i2cGetErrors(&driver);}
	
	msg_t transmit(i2caddr_t addr, const uint8_t *txbuf, size_t txbytes,
	               uint8_t *rxbuf, size_t rxbytes, systime_t timeout);
	
	msg_t receive(i2caddr_t addr, uint8_t * rxbuf, size_t rxbytes,
	              systime_t timeout);

	uint8_t read_byte(i2caddr_t addr, uint8_t regaddr);
	
	msg_t write_byte(i2caddr_t addr, uint8_t regaddr, uint8_t value);
	
protected:
	//! Driver configuration
	I2CConfig const config;
	//! Count of tasks dependent on this driver
	uint32_t refcnt;
	//! ChibiOS I2C driver
	I2CDriver &driver;
};

//! @} @}

#endif
