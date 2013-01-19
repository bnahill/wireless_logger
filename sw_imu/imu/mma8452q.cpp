/*!
 @file mma8452q.cpp
 
 @brief 
 
 @author Ben Nahill <bnahill@gmail.com>
 */

#include "mma8452q.h"

#include "platform.h"

bool MMA8452Q::init(){
	chSemInit(&result_lock, 1);
	
	// Initialize I2C -- don't care about return value
	i2c.init();
	
	reset();
	
	i2c.read_byte(devaddr, REG_WHO_AM_I);
	if(i2c.read_byte(devaddr, REG_WHO_AM_I) != 0x2A){
		while(1);
		return false;
	}
	
	//i2c.write_byte(devaddr, REG_CTRL_REG2, 0x40);
	
	// Set for +/-4g
	i2c.write_byte(devaddr, REG_XYZ_DATA_CFG, 0x01);
	i2c.write_byte(devaddr, REG_SYSMOD, 0x02);
	
	// Enable device
	//i2c.write_byte(devaddr, REG_CTRL_REG1, 0x21);
	
	update_ctrl_regs();
	
	return true;
}

void MMA8452Q::reset(){
	i2c.write_byte(devaddr, REG_CTRL_REG1, 0x40);
	chThdSleep(MS2ST(1));
}


void MMA8452Q::read(){
	static const uint8_t addr = REG_OUT_X_MSB;
	i2c.transmit(devaddr, &addr, 1, xfer_buffer, 6, MS2ST(4));
	chSemWait(&result_lock);
	reading.x = ((float)((int16_t)(xfer_buffer[0] << 8) + xfer_buffer[1]) / (512.0 * 16));
	reading.y = ((float)((int16_t)(xfer_buffer[2] << 8) + xfer_buffer[3]) / (512.0 * 16));
	reading.z = ((float)((int16_t)(xfer_buffer[4] << 8) + xfer_buffer[5]) / (512.0 * 16));
	chSemSignal(&result_lock);
}


void MMA8452Q::update_ctrl_regs(){
	static uint8_t tx[3] = {REG_CTRL_REG1};
	// REG1
	tx[1] = (aslp_dr << 5) | (dr << 3) | (noise_mode << 2) |
	          (fast_read << 1) | active_mode;
	// REG2
	tx[2] = (oversmp_mode_sleep << 3) | (autosleep << 2) |
	           oversmp_mode_active;
	
	i2c.transmit(devaddr, tx, 3, NULL, 0, TIME_INFINITE);
}

void MMA8452Q::set_dr(dr_t new_dr, bool update){
	dr = new_dr;
	if(update)
		update_ctrl_regs();
}
