/*!
 @file mag3110.cpp
 @brief Body for \ref MAG3110 class
 @author Ben Nahill <bnahill@gmail.com>
 */

#include "mag3110.h"

#include "platform.h"

bool MAG3110::init(){
	chSemInit(&result_lock, 1);
	
	// Initialize I2C -- don't care about return value
	i2c.init();
	i2c.read_byte(devaddr, REG_WHO_AM_I);
	if(i2c.read_byte(devaddr, REG_WHO_AM_I) != whoami){
		return false;
	}
	
	write_ctrl_reg1();
	write_ctrl_reg2();
	
	return true;
}

void MAG3110::write_ctrl_reg1(){
	uint8_t value;
	value = (dr << 5) | (os << 3) | 0 | 0 | mode;
	i2c.write_byte(devaddr, REG_CTRL_REG1, value);
}

void MAG3110::write_ctrl_reg2(){
	uint8_t value;
	value = (autoreset_mode << 7) | (raw_mode << 5);
	i2c.write_byte(devaddr, REG_CTRL_REG2, value);
}

void MAG3110::read(){
	static const uint8_t addr = REG_OUT_X_MSB;
	i2c.transmit(devaddr, &addr, 1, xfer_buffer, 6, MS2ST(4));
	chSemWait(&result_lock);
	reading.x = ((float)((int16_t)(xfer_buffer[0] << 8) + xfer_buffer[1]) * 0.10);
	reading.y = ((float)((int16_t)(xfer_buffer[2] << 8) + xfer_buffer[3]) * 0.10);
	reading.z = ((float)((int16_t)(xfer_buffer[4] << 8) + xfer_buffer[5]) * 0.10);
	chSemSignal(&result_lock);
}

