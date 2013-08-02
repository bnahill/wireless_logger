/*!
 @file i2c.cpp
 @brief Body for \ref I2C class
 @author Ben Nahill <bnahill@gmail.com>
 */

#include "imu/i2c.h"

I2C::I2C(I2CDriver& driver, i2copmode_t opmode,
         i2cdutycycle_t dutycycle, uint32_t speed,
         gpio_pin_t const &sda, gpio_pin_t const &scl) :
	driver(driver),
	refcnt(0),
	config({opmode,speed,dutycycle}),
	sda(sda), scl(scl){}
	
void I2C::init(){
	lock();
	if(!(refcnt++)){
		clear_bus();
		clk_mgr_req_hsi();
		//i2cObjectInit(&driver);
		i2cStart(&driver, &config);
		clk_mgr_noreq_hsi();
	}
	unlock();
}

void I2C::close(){
	if(!(--refcnt)){
		clk_mgr_req_hsi();
		i2cStop(&driver);
		clk_mgr_noreq_hsi();
	}
}

void I2C::clear_bus(){
	scl.set_mode(gpio_pin_t::MODE_OUTPUT);
	for(uint32_t i = 0; i < 100; i++){
		scl.set();
		chThdSleep(1);
		scl.clear();
		chThdSleep(1);
	}
	scl.set_mode(gpio_pin_t::MODE_ALT);
}


void I2C::reconfig_clock(){
	i2cStop(&driver);
	i2cStart(&driver, &config);
}


msg_t I2C::transmit(i2caddr_t addr, const uint8_t* txbuf, size_t txbytes,
                    uint8_t* rxbuf, size_t rxbytes, systime_t timeout){
	msg_t msg;
	lock();
// 	msg = i2cMasterTransmitTimeout(&driver, addr, txbuf, txbytes, rxbuf,
// 	                               rxbytes, timeout);
	clk_mgr_req_hsi();
	msg = i2cMasterTransmitTimeout(&driver, addr, txbuf, txbytes, rxbuf,
	                               rxbytes, MS2ST(txbytes+rxbytes));
	if(msg == RDY_TIMEOUT){
		i2cStop(&driver);
		clear_bus();
		i2cStart(&driver, &config);
	}
	clk_mgr_noreq_hsi();
	unlock();
	return msg;
}


msg_t I2C::receive(i2caddr_t addr, uint8_t* rxbuf, size_t rxbytes,
                   systime_t timeout){
	msg_t msg;
	lock();
	//msg = i2cMasterReceiveTimeout(&driver, addr, rxbuf, rxbytes, timeout);
	clk_mgr_req_hsi();
	msg = i2cMasterReceiveTimeout(&driver, addr, rxbuf, rxbytes, MS2ST(rxbytes));
	if(msg == RDY_TIMEOUT){
		i2cStop(&driver);
		clear_bus();
		i2cStart(&driver, &config);
	}
	clk_mgr_noreq_hsi();
	unlock();
	return msg;
}

uint8_t I2C::read_byte_test(i2caddr_t addr, uint8_t regaddr){
	msg_t msg;
	uint8_t ret;
	msg = transmit(addr, &regaddr, 1, &ret, 1, MS2ST(4));
	if(msg == RDY_TIMEOUT){
		ret = 0;
		i2cAcquireBus(&driver);
		driver.i2c->CR1 |= I2C_CR1_START;
		chThdSleep(MS2ST(1));
		if((driver.i2c->SR2 & I2C_SR2_MSL) == 0){
			// I2C is messed up
			scl.set_mode(gpio_pin_t::MODE_OUTPUT);
			for(uint32_t i = 0; i < 10; i++){
				scl.clear();
				chThdSleep(MS2ST(1));
				scl.set();
				chThdSleep(MS2ST(1));
			}
			scl.set_mode(gpio_pin_t::MODE_ALT);
		}
		while(driver.i2c->SR1 & I2C_SR1_TXE);
		i2cReleaseBus(&driver);
		msg = transmit(addr, &regaddr, 1, &ret, 1, MS2ST(4));
		if(msg == RDY_TIMEOUT){
			ret = 0;
		}
	}
	return ret;
	
}

uint8_t I2C::read_byte(i2caddr_t addr, uint8_t regaddr){
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

msg_t I2C::write_byte(i2caddr_t addr, uint8_t regaddr, uint8_t value){
	uint8_t to_transmit[2] = {regaddr, value};
	msg_t msg;
	msg = transmit(addr, to_transmit, 2, NULL, 0, TIME_INFINITE);
	return msg;
}