/*!
 @file mma845x.cpp
 @brief Body for Freescale \ref MMA845x accelerometer class
 @author Ben Nahill <bnahill@gmail.com>
 */

#include "mma845x.h"

#include "platform.h"

template<mma845x_variant_t variant>
bool MMA845x< variant >::init(){
	// Initialize I2C -- don't care about return value
	i2c.init();
	
	i2c.read_byte(devaddr, REG_WHO_AM_I);
	if(i2c.read_byte(devaddr, REG_WHO_AM_I) != whoami_id()){
		return false;
	}
	
	reset();
	
	//i2c.write_byte(devaddr, REG_CTRL_REG2, 0x40);
	
	// Set full-scale range
	i2c.write_byte(devaddr, REG_XYZ_DATA_CFG, fs);
	i2c.write_byte(devaddr, REG_SYSMOD, 0x02);
	
	update_ctrl_regs();
	
	return true;
}

template<mma845x_variant_t variant>
void MMA845x< variant >::reset(){
	i2c.write_byte(devaddr, REG_CTRL_REG1, 0x40);
	chThdSleep(MS2ST(1));
}

template<mma845x_variant_t variant>
void MMA845x< variant >::read(){
	static const uint8_t addr = REG_OUT_X_MSB;
	i2c.transmit(devaddr, &addr, 1, xfer_buffer, 6, MS2ST(4));
	__disable_irq();
	reading.x = ((float)((int16_t)(xfer_buffer[0] << 8) + xfer_buffer[1])) * scaling_factor;
	reading.y = ((float)((int16_t)(xfer_buffer[2] << 8) + xfer_buffer[3])) * scaling_factor;
	reading.z = ((float)((int16_t)(xfer_buffer[4] << 8) + xfer_buffer[5])) * scaling_factor;
	__enable_irq();
}

template<mma845x_variant_t variant>
void MMA845x< variant >::update_ctrl_regs(){
	static uint8_t tx[3] = {REG_CTRL_REG1};
	// REG1
	tx[1] = (aslp_dr << 5) | (dr << 3) | (noise_mode << 2) |
	          (fast_read << 1) | active_mode;
	// REG2
	tx[2] = (oversmp_mode_sleep << 3) | (autosleep << 2) |
	           oversmp_mode_active;
	
	i2c.transmit(devaddr, tx, 3, NULL, 0, TIME_INFINITE);
}

template<mma845x_variant_t variant>
void MMA845x< variant >::set_dr(dr_t new_dr, bool update){
	dr = new_dr;
	if(update)
		update_ctrl_regs();
}

template<mma845x_variant_t variant>
void MMA845x< variant >::set_fs(fs_t new_fs, bool update){
	fs = new_fs;
	if(update)
		i2c.write_byte(devaddr, REG_XYZ_DATA_CFG, fs);
}
