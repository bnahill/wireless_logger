#ifndef __LSM303_H_
#define __LSM303_H_

#include "stm32f10x.h"
#include "sensor_config.h"
#include "i2c.h"

typedef enum {
	LSM_ACC_RATE_50   = 0,
	LSM_ACC_RATE_100  = 1,
	LSM_ACC_RATE_400  = 3,
	LSM_ACC_RATE_1000 = 4
} lsm_acc_rate_t;

typedef enum {
	LSM_MAG_RATE_0_75 = 0,
	LSM_MAG_RATE_1_5  = 1,
	LSM_MAG_RATE_3    = 2,
	LSM_MAG_RATE_7_5  = 3,
	LSM_MAG_RATE_15   = 4,
	LSM_MAG_RATE_30   = 5,
	LSM_MAG_RATE_75   = 6
} lsm_mag_rate_t;

typedef enum {
	LSM_PM_OFF    = 0,
	LSM_PM_NORM   = 1,
	LSM_PM_LP_0_5 = 2,
	LSM_PM_LP_1   = 3,
	LSM_PM_LP_2   = 4,
	LSM_PM_LP_5   = 5,
	LSM_PM_LP_10  = 6
} lsm_pm_t;

typedef enum {
	LSM_ACC_FS_2G  = 0,
	LSM_ACC_FS_4G  = 1,
	LSM_ACC_FS_8G  = 2
} lsm_acc_fs_t;

typedef enum {
	LSM_MAG_FS_NONE  = 0,
	LSM_MAG_FS_1_3   = 1,
	LSM_MAG_FS_1_9   = 2,
	LSM_MAG_FS_2_5   = 3,
	LSM_MAG_FS_4     = 4,
	LSM_MAG_FS_4_7   = 5,
	LSM_MAG_FS_5_6   = 6,
	LSM_MAG_FS_8_1   = 7
} lsm_mag_fs_t;

typedef struct {
	euclidean3_t   mag;
	euclidean3_t   acc;
	i2c_t *const   i2c;
	lsm_acc_rate_t acc_rate;
	lsm_mag_rate_t mag_rate;
	lsm_acc_fs_t   acc_fs;
	lsm_mag_fs_t   mag_fs;
	lsm_pm_t       pow_mode;
	i2c_transfer_t acc_xfer, mag_xfer;
	uint8_t        acc_buff[6], mag_buff[6];
} lsm303_t;

#if HAS_MAGACC
extern lsm303_t magacc;
#endif

void lsm303_init(void);
void lsm303_read(void);
int lsm303_xfer_complete(void);
void lsm303_update(void);
void lsm303_read_sync(void);

#endif

