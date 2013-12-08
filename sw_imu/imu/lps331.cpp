#include "imu/lps331.h"

bool LPS331::init(){
	spi.init();
	if(read_register(REG_WHOAMI) != whoami){
		return false;
	}

	return true;
}

float LPS331::read(){
	uint8_t const tx[6] = {0x80 | 0x40 | REG_PRS_OUT_XL, 0, 0, 0, 0, 0};
	uint8_t rx[6];
	spi.exchange_sync(spi_config, 6, tx, rx);


	if(rx[3] & 0x80){
		pressure = (float)((int32_t)(0xFF | (rx[3] << 16) | (rx[2] << 8) | rx[1])) * scaling_prs;
	} else {
		pressure = (float)((int32_t)((rx[3] << 16) | (rx[2] << 8) | rx[1])) * scaling_prs;
	}
	temperature = (float)((int16_t)((rx[5] << 8) | rx[4])) * scaling_temp + offset_temp;

	return pressure;
}

uint8_t LPS331::read_register(reg_t reg){
	uint8_t tx[2];
	uint8_t rx[2];

	rx[0] = 0x11;
	rx[1] = 0x22;

	tx[0] = 0x80 | 0x04 | reg;
	tx[1] = 0;

	spi.exchange_sync(spi_config, 2, tx, rx);
	return rx[1];
}
