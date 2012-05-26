#include "stm32f10x.h"
#include "i2c.h"
#include "lsm303.h"

//! @name I2C Addresses
//! @{
#define ACC_ADDR 0x33
#define MAG_ADDR 0x3D
//! @}

//! @name Accelerometer register addresses
//! @{
#define ACC_REG_CTRL1     0x20
#define ACC_REG_CTRL2     0x21
#define ACC_REG_CTRL3     0x22
#define ACC_REG_CTRL4     0x23
#define ACC_REG_CTRL5     0x24
#define ACC_REG_CTRL6     0x25
#define ACC_REG_REFA      0x26
#define ACC_REG_STAT      0x27
#define ACC_REG_X_L       0x28
#define ACC_REG_X_H       0x29
#define ACC_REG_Y_L       0x2A
#define ACC_REG_Y_H       0x2B
#define ACC_REG_Z_L       0x2C
#define ACC_REG_Z_H       0x2D
#define ACC_REG_FIFO_CTL  0x2E
#define ACC_REG_FIFO_SRC  0x2F
#define ACC_REG_INT1_CFG  0x30
#define ACC_REG_INT1_SRC  0x31
#define ACC_REG_INT1_THS  0x32
#define ACC_REG_INT1_DUR  0x33
#define ACC_REG_INT2_CFG  0x34
#define ACC_REG_INT2_SRC  0x35
#define ACC_REG_INT2_THS  0x36
#define ACC_REG_INT2_DUR  0x37
#define ACC_REG_CLCK_CFG  0x38
#define ACC_REG_CLCK_SRC  0x39
#define ACC_REG_CLCK_THS  0x3A
#define ACC_REG_TIME_LIM  0x3B
#define ACC_REG_TIME_LAT  0x3C
#define ACC_REG_TIME_WIN  0x3D
//! }@

//! @name Magnetometer register addresses
//! @{
#define MAG_REG_CRA       0x00
#define MAG_REG_CRB       0x01
#define MAG_REG_MR        0x02
#define MAG_REG_X_H       0x03
#define MAG_REG_X_L       0x04
#define MAG_REG_Z_H       0x05
#define MAG_REG_Z_L       0x06
#define MAG_REG_Y_H       0x07
#define MAG_REG_Y_L       0x08
#define MAG_REG_SR        0x09
#define MAG_REG_IRA       0x0A
#define MAG_REG_IRB       0x0B
#define MAG_REG_IRC       0x0C
#define MAG_REG_TEMP_H    0x31
#define MAG_REG_TEMP_L    0x32
//! @}

const float acc_scale[] = {
	1.0 / 1000.0,
	2.0 / 1000.0,
	3.9 / 1000.0
};

const float mag_scale_xy[] = {
	0,
	1.0 / 1055.0,
	1.0 / 795.0,
	1.0 / 635.0,
	1.0 / 430.0,
	1.0 / 375.0,
	1.0 / 320.0,
	1.0 / 230.0
};

const float mag_scale_z[] = {
	0,
	1.0 / 950.0,
	1.0 / 710.0,
	1.0 / 570.0,
	1.0 / 385.0,
	1.0 / 335.0,
	1.0 / 285.0,
	1.0 / 205.0
};

#if HAS_MAGACC
lsm303_t magacc = {
	{0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0},
	&i2c1,
	LSM_ACC_RATE_100,
	LSM_MAG_RATE_75,
	LSM_ACC_FS_4G,
	LSM_MAG_FS_4,
	LSM_PM_NORM
};
#endif

void lsm303_set_acc_fs(lsm303_t *lsm, lsm_acc_fs_t fs){
	uint8_t current = i2c_read_byte(lsm->i2c, ACC_ADDR, ACC_REG_CTRL4 | 0x80) & 0xCF;
	current |= (fs & 0x3) << 4;
	i2c_write_byte(lsm->i2c, ACC_ADDR, ACC_REG_CTRL4, current);
	lsm->acc_fs = fs;
}

void lsm303_set_acc_rate(lsm303_t *lsm, lsm_acc_rate_t acc_rate){
	lsm->acc_rate = acc_rate;
	i2c_write_byte(lsm->i2c, ACC_ADDR, ACC_REG_CTRL1, (lsm->pow_mode << 5) | (lsm->acc_rate << 3) | 0x07);
}

static void lsm303_device_init(lsm303_t *lsm){
	// Init CTRL1-6
	uint8_t conf_buffer[6];
	i2c_transfer_t xfer;
	
	conf_buffer[0] = (lsm->pow_mode << 5) | (lsm->acc_rate << 3) | 0x07;
	conf_buffer[1] = 0;
	conf_buffer[2] = 0;
	conf_buffer[3] = 0x80 | (lsm->acc_fs << 4);
	conf_buffer[4] = 0;
	conf_buffer[5] = 0;
	
	i2c_mk_transfer(&xfer,
	                I2C_OP_WRITE,
	                ACC_ADDR,
	                ACC_REG_CTRL1 | 0x80,
	                conf_buffer,
	                6
	);

	// Initialize I2C device
	i2c_init(lsm->i2c, I2C_MODE_MASTER, 400000);

	// Apply accelerometer configuration
	i2c_transfer(lsm->i2c, &xfer);
	while(!xfer.done);

	xfer.devaddr = MAG_ADDR;
	xfer.addr = MAG_REG_CRA | 0x80;
	xfer.buffer = conf_buffer;
	conf_buffer[0] = lsm->mag_rate << 2;
	conf_buffer[1] = lsm->mag_fs << 5;
	conf_buffer[2] = 0;
	xfer.count = 3;

	// Apply magnetometer configuration
	i2c_transfer(lsm->i2c, &xfer);
	while(!xfer.done);
}

static void lsm303_do_read(lsm303_t *lsm){
	i2c_mk_transfer(&lsm->mag_xfer,
	                I2C_OP_READ,
	                MAG_ADDR,
	                MAG_REG_X_H | 0x80,
	                lsm->mag_buff,
	                6
	);
	
	i2c_mk_transfer(&lsm->acc_xfer,
	                I2C_OP_READ,
	                ACC_ADDR,
	                ACC_REG_X_L | 0x80,
	                lsm->acc_buff,
	                6
	);
	
	// Add the mag transfer as a followup from the acc transfer
	lsm->acc_xfer.next = &lsm->mag_xfer;
	
	// Start the acc transfer
	i2c_transfer(lsm->i2c, &lsm->acc_xfer);
}

int lsm303_xfer_complete(void){
#if HAS_MAGACC
	if(!magacc.mag_xfer.done || !magacc.acc_xfer.done)
		return 0;
#endif
	return 1;	
}

void lsm303_read_sync(void){
	lsm303_read();
	while(!lsm303_xfer_complete());
	lsm303_update();
}

void lsm303_do_update(lsm303_t *lsm){
	int16_t tmp16;
	tmp16 = lsm->acc_buff[0] | (lsm->acc_buff[1] << 8);
	lsm->acc.x = tmp16 * acc_scale[lsm->acc_fs];
	tmp16 = lsm->acc_buff[2] | (lsm->acc_buff[3] << 8);
	lsm->acc.y = tmp16 * acc_scale[lsm->acc_fs];
	tmp16 = lsm->acc_buff[4] | (lsm->acc_buff[5] << 8);
	lsm->acc.z = tmp16 * acc_scale[lsm->acc_fs];
	
	tmp16 = lsm->mag_buff[1] | (lsm->mag_buff[0] << 8);
	lsm->mag.x = tmp16 * mag_scale_xy[lsm->mag_fs];
	tmp16 = lsm->mag_buff[3] | (lsm->mag_buff[2] << 8);
	lsm->mag.z = tmp16 * mag_scale_xy[lsm->mag_fs];
	tmp16 = lsm->mag_buff[5] | (lsm->mag_buff[4] << 8);
	lsm->mag.y = tmp16 * mag_scale_z[lsm->mag_fs];
}


void lsm303_update(void){
#if HAS_MAGACC
	lsm303_do_update(&magacc);
#endif	
}

void lsm303_init(void){
#if HAS_MAGACC
	lsm303_device_init(&magacc);
#endif
}

void lsm303_read(void){
#if HAS_MAGACC
	lsm303_do_read(&magacc);
#endif
}

