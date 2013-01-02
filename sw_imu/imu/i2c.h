#ifndef __IMU_I2C_H_
#define __IMU_I2C_H_

#include "ch.h"
#include "hal.h"

class I2C {
public:
	/*!
	 @brief Constructor for I2C driver. Statically initialize with your settings.
	 @param driver The ChibiOS I2C HAL driver
	 @param opmode The I2C mode for the ChibiOS driver
	 @param dutycycle The clock duty cycle to use
	 @param speed The clock rate in Hz
	 */
	I2C(I2CDriver &driver, i2copmode_t opmode, i2cdutycycle_t dutycycle, uint32_t speed) :
		driver(driver),
		refcnt(0),
		config({opmode,speed,dutycycle})
	{}
	
	/*!
	 @brief Start the driver. If it is already started it will merely count a
	 new reference.
	 */
	void init(){
		if(!(refcnt++)){
			i2cObjectInit(&driver);
			i2cStart(&driver, &config);
		}
	}
	
	/*!
	 @brief Close the driver. It will only close once all references have
	 closed.
	 */
	void close(){
		if(!(--refcnt)){
			i2cStop(&driver);
		}
	}
	
	/*!
	 @brief Lock the mutex for the driver
	 */
	void acquire(){
		i2cAcquireBus(&driver);
	}
	
	/*!
	 @brief Release the mutex for the driver
	 */
	void release(){
		i2cReleaseBus(&driver);
	}
	
	/*!
	 @brief Get the errors that were encountered
	 @return I2C Error flags
	 */
	i2cflags_t get_errors(){
		return i2cGetErrors(&driver);
	}
	
	msg_t transmit(i2caddr_t addr, const uint8_t *txbuf, size_t txbytes, uint8_t *rxbuf, size_t rxbytes, systime_t timeout){
		msg_t msg;
		acquire();
		msg = i2cMasterTransmitTimeout(&driver, addr, txbuf, txbytes, rxbuf, rxbytes, timeout);
		if(msg == RDY_TIMEOUT){
			i2cStop(&driver);
			i2cStart(&driver, &config);
		}
		release();
		return msg;
	}
	
	msg_t receive(i2caddr_t addr, uint8_t * rxbuf, size_t rxbytes, systime_t timeout){
		msg_t msg;
		acquire();
		msg = i2cMasterReceiveTimeout(&driver, addr, rxbuf, rxbytes, timeout);
		if(msg == RDY_TIMEOUT){
			i2cStop(&driver);
			i2cStart(&driver, &config);
		}
		release();
		return msg;
	}

	uint8_t read_byte(i2caddr_t addr, uint8_t regaddr){
		uint8_t ret;
		msg_t msg;
		i2cflags_t flags;
		
		
		msg = transmit(addr, &regaddr, 1, &ret, 1, MS2ST(4));
		
		if(msg == RDY_OK){
			return ret;
		} else {
			flags = get_errors();
			return 0;
		}
	}
	
	msg_t write_byte(i2caddr_t addr, uint8_t regaddr, uint8_t value){
		uint8_t to_transmit[2] = {regaddr, value};
		msg_t msg;
		msg = transmit(addr, to_transmit, 2, NULL, 0, TIME_INFINITE);
		return msg;
	}
	
protected:
	//! Driver configuration
	I2CConfig const config;
	//! Count of tasks dependent on this driver
	uint32_t refcnt;
	//! ChibiOS I2C driver
	I2CDriver &driver;
};

#endif
