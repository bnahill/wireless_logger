#include "imu/i2c.h"

I2C::I2C(I2CDriver& driver, i2copmode_t opmode,
         i2cdutycycle_t dutycycle, uint32_t speed) :
	driver(driver),
	refcnt(0),
	config({opmode,speed,dutycycle}){}
	
void I2C::init(){
	if(!(refcnt++)){
		i2cObjectInit(&driver);
		i2cStart(&driver, &config);
	}
}

void I2C::close(){
	if(!(--refcnt)){
		i2cStop(&driver);
	}
}

msg_t I2C::transmit(i2caddr_t addr, const uint8_t* txbuf, size_t txbytes,
                    uint8_t* rxbuf, size_t rxbytes, systime_t timeout){
	msg_t msg;
	acquire();
	msg = i2cMasterTransmitTimeout(&driver, addr, txbuf, txbytes, rxbuf,
	                               rxbytes, timeout);
	if(msg == RDY_TIMEOUT){
		i2cStop(&driver);
		i2cStart(&driver, &config);
	}
	release();
	return msg;
}


msg_t I2C::receive(i2caddr_t addr, uint8_t* rxbuf, size_t rxbytes,
                   systime_t timeout){
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