#include "stm32f10x.h"
#include "lps001.h"

//! @addtogroup lps
//! @{

//! The I2C address
#define LPS_ADDR 0xBA

//! @name Register Addresses
//! @{
#define LPS_REG_WHOAMI    0x0F
#define LPS_REG_CTRL1     0x20
#define LPS_REG_CTRL2     0x21
#define LPS_REG_CTRL3     0x22
#define LPS_REG_STATUS    0x27
#define LPS_REG_PRESS_L   0x28
#define LPS_REG_PRESS_H   0x29
#define LPS_REG_TEMP_L    0x2A
#define LPS_REG_TEMP_H    0x2B
#define LPS_REG_D_PRESS_L 0x2C
#define LPS_REG_D_PRESS_H 0x2D
#define LPS_REG_R_PRESS_L 0x30
#define LPS_REG_R_PRESS_H 0x31
#define LPS_REG_THS_P_L   0x32
#define LPS_REG_THS_P_H   0x33
#define LPS_REG_INT_CFG   0x34
#define LPS_REG_INT_SRC   0x35
#define LPS_REG_INT_ACK   0x36
//! @}

//! @name Sensitivity values
//! @{

//! The pressure sensor sensitivity in mbar/LSB
#define LPS_PRESSURE_MBAR_LSB (1.0 / 16.0)
//! The temperature sensor sensitivity in degC/LSB
#define LPS_TEMP_C_LSB        (1.0 / 64.0)
//! @}

//! The pressure sensor instance
lps001_t pressure = {
	// I2C device
	&i2c2,
	// Pressure reading
	0.0,
	// Temperature reading
	0.0
};

//! @}

void lps_init(void){
	i2c_init(pressure.i2c, I2C_MODE_MASTER, 400000);
	if(i2c_read_byte(pressure.i2c, LPS_ADDR, LPS_REG_WHOAMI) != 0xBA)
		while(1);
	
	i2c_write_byte(pressure.i2c, LPS_ADDR, LPS_REG_CTRL1, 0x74);
}

void lps_read(void){
	i2c_mk_transfer(&pressure.xfer,
	                I2C_OP_READ,
	                LPS_ADDR,
	                LPS_REG_PRESS_L | 0x80,
	                pressure.buffer,
	                4
	);
	i2c_transfer(pressure.i2c, &pressure.xfer);
}

int lps_xfer_complete(void){
	if(pressure.xfer.done)
		return 1;
	return 0;
}

void lps_update(void){
	int16_t tmp;
	tmp = pressure.buffer[0] | (pressure.buffer[1] << 8);
	pressure.pressure = tmp * LPS_PRESSURE_MBAR_LSB;
	tmp = pressure.buffer[2] | (pressure.buffer[3] << 8);
	pressure.temperature = tmp * LPS_TEMP_C_LSB;
}

void lps_read_sync(void){
	lps_read();
	while(!lps_xfer_complete());
	lps_update();
}
