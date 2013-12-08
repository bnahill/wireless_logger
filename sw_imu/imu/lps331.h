#ifndef __IMU_LPS331_H_
#define __IMU_LPS331_H_

#include "imu/imu.h"
#include "imu/spi.h"


class LPS331 {
public:
	LPS331(SPI &spi, SPI::slave_config_t const spi_config) :
		spi(spi), spi_config(spi_config), odr(ODR_12_5Hz), pm(PM_DOWN)
	{

	}

	typedef enum {
		ODR_ONESHOT = 0,
		ODR_1Hz = 1,
		ODR_7Hz = 2,
		ODR_12_5Hz = 3,
		ODR_25Hz = 4,
		ODR_7Hz_FASTTEMP = 5,
		ODR_12_5Hz_FASTTEMP = 6,
		ODR_25Hz_FASTTEMP = 7
	} odr_t;

	typedef enum {
		PM_DOWN = 0,
		PM_ACTIVE = 1
	} power_mode_t;

	bool init();

	void set_odr(odr_t new_odr){
		odr = new_odr;
		write_register(REG_CTRL_REG1, (pm << 7) | (odr << 4) | 4);
	}

	void set_pm(power_mode_t new_pm){
		pm = new_pm;
		write_register(REG_CTRL_REG1, (pm << 7) | (odr << 4) | 4);
	}

	float read();

	static char * format_str(char * dst){
		dst = imu_sprint(dst, 1) + 1;
		dst = imu_sprint(dst, "pressure") + 1;
		return get_type_str<decltype(pressure)>(dst);
	}
protected:
	typedef enum {
		REG_REF_P_XL    = 0x08,
		REG_P_L         = 0x09,
		REG_P_H         = 0x0A,
		REG_WHOAMI      = 0x0F,
		REG_RES_CONF    = 0x10,
		REG_CTRL_REG1   = 0x20,
		REG_CTRL_REG2   = 0x21,
		REG_CTRL_REG3   = 0x22,
		REG_INT_CFG     = 0x23,
		REG_INT_SRC     = 0x24,
		REG_THS_P_LOW   = 0x25,
		REG_THS_P_HI    = 0x26,
		REG_STATUS      = 0x27,
		REG_PRS_OUT_XL  = 0x28,
		REG_PRS_OUT_L   = 0x29,
		REG_PRS_OUT_H   = 0x2A,
		REG_TEMP_OUT_L  = 0x2B,
		REG_TEMP_OUT_H  = 0x2C,
		REG_AMP_CTRL    = 0x30
	} reg_t;

	uint8_t read_register(reg_t reg);

	void write_register(reg_t reg, uint8_t value){
		uint8_t const tx[2] = {reg,value};

		spi.send_sync(spi_config, 2, tx);
	}

	SPI &spi;
	SPI::slave_config_t const spi_config;

	float temperature;
	float pressure;

	odr_t odr;
	power_mode_t pm;

	static constexpr uint8_t whoami = 0xBB;
	static constexpr float scaling_prs = 1.0/4096.0;
	static constexpr float offset_temp = 46.5;
	static constexpr float scaling_temp = 1.0/480.0;
};

#endif // __IMU_LPS331_H_
