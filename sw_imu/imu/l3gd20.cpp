#ifndef __IMU_L3GD20_H_
#error "This file must be included by l3gd20.h"
#endif

// This inclusion is really just for IDEs to know that the definitions from
// l3gd20.h are available
#include "l3gd20.h"

template < class spi_class >
L3GD20< spi_class >::L3GD20(spi_class& spi, typename spi_class::slave_config_t spi_config): 
		spi(spi), spi_config(spi_config),
		odr(ODR_95), lp_cutoff(LP_3), power_mode(POWER_DOWN),
		hp_mode(HP_MODE_NORMAL_RST), fs(FS_500), irq_drive(DRIVE_PUSH_PULL),
		interrupts(0), dps_scale(get_dps_scale(FS_500)), bdu_mode(false),
		endian_mode(ENDIAN_BIG), fifo_enabled(false), hp_enabled(false),
		int1_selection(SIG_SRC_LPF1), output_selection(SIG_SRC_LPF1)
	{}

template < class spi_class >
bool L3GD20< spi_class >::init(){
	chSemInit(&result_lock, 1);
	
	spi.init();
	
	if(spi_read_reg(REG_WHO_AM_I) != whoami)
		while(1);
	
	update_ctrl_regs();
	
	return true;
}

template < class spi_class >
void L3GD20< spi_class >::read(){
	uint8_t rx_buff[7];
	uint8_t tx_buff;
	int16_t tmp;
	
	tx_buff = 0x80 | 0x40 | REG_OUT_X_H;
	
	spi.exchange_sync(spi_config, 7, &tx_buff, rx_buff);
	chSemWait(&result_lock);
	tmp = (rx_buff[2] << 8) | rx_buff[1];
	reading.x = (float)tmp * dps_scale;
	tmp = (rx_buff[4] << 8) | rx_buff[3];
	reading.y = (float)tmp * dps_scale;
	tmp = (rx_buff[6] << 8) | rx_buff[5];
	reading.z = (float)tmp * dps_scale;
	chSemSignal(&result_lock);
}

template < class spi_class >
int8_t L3GD20< spi_class >::read_temperature(){
	temperature = spi_read_reg(REG_OUT_TEMP);
	return temperature;
}

template < class spi_class >
void L3GD20< spi_class >::set_power_mode(power_mode_t new_pm, bool write){
	power_mode = new_pm;
	if(write) update_reg_ctrl1(true);
}

template < class spi_class >
void L3GD20< spi_class >::set_full_scale(fs_t new_fs, bool write){
	fs = new_fs;
	dps_scale = get_dps_scale(fs);
	if(write) update_reg_ctrl2(true);
}

template < class spi_class >
void L3GD20< spi_class >::spi_read(reg_t addr, size_t n, uint8_t *dst){
	addr = (reg_t) (addr | FLAG_READ | FLAG_SEQ);
	spi.exchange_sync(spi_config, n, &addr, dst);
}

template < class spi_class >
uint8_t L3GD20< spi_class >::spi_read_reg(reg_t addr){
	uint8_t rx_buff[2];
	uint8_t tx_buff[2];
	tx_buff[0] = ((uint8_t)addr) | FLAG_READ;
	tx_buff[1] = 0;
	spi.exchange_sync(spi_config, 2, tx_buff, rx_buff);
	return rx_buff[1];
}

template < class spi_class >
void L3GD20< spi_class >::spi_write_reg(reg_t addr, uint8_t value){
	uint8_t rx_buff[2];
	uint8_t tx_buff[2];
	tx_buff[0] = addr;
	tx_buff[1] = value;
	spi.exchange_sync(spi_config, 2, tx_buff, rx_buff);
}

template < class spi_class >
uint8_t L3GD20< spi_class >::update_reg_ctrl1(bool write){
	uint8_t reg = (odr << 6) | (lp_cutoff << 4) | (power_mode << 3) | 7;
	if(write) spi_write_reg(REG_CTRL_REG1, reg);
	return reg;
}

template < class spi_class >
uint8_t L3GD20< spi_class >::update_reg_ctrl2(bool write){
	uint8_t reg;
	if(hp_cutoff > 9) hp_cutoff = 9;
	reg = (hp_mode << 4) | hp_cutoff;
	if(write) spi_write_reg(REG_CTRL_REG2, reg);
	return reg;
}

template < class spi_class >
uint8_t L3GD20< spi_class >::update_reg_ctrl3(bool write){	
	uint8_t reg;
	interrupts &= ~(0x30);
	interrupts |= (irq_polarity << 5) | (irq_drive << 4);
	reg = (hp_mode << 4) | hp_cutoff;
	if(write) spi_write_reg(REG_CTRL_REG3, reg);
	return reg;
}

template < class spi_class >
uint8_t L3GD20< spi_class >::update_reg_ctrl4(bool write){	
	uint8_t reg;
	reg = (bdu_mode ? 0x80 : 0) | (endian_mode << 6) | (fs << 4);
	if(write) spi_write_reg(REG_CTRL_REG4, reg);
	return reg;
}

template < class spi_class >
uint8_t L3GD20< spi_class >::update_reg_ctrl5(bool write){	
	uint8_t reg;
	reg = (fifo_enabled ? 0x40:0) | (hp_enabled ? 0x20:0) |
			(int1_selection << 2) | (output_selection);
	if(write) spi_write_reg(REG_CTRL_REG5, reg);
	return reg;
}

template < class spi_class >
void L3GD20< spi_class >::update_ctrl_regs(){
	uint8_t tx_buff[6], rx_buff[6];
	tx_buff[0] = REG_CTRL_REG1 | FLAG_SEQ;
	tx_buff[1] = update_reg_ctrl1(false);
	tx_buff[2] = update_reg_ctrl2(false);
	tx_buff[3] = update_reg_ctrl3(false);
	tx_buff[4] = update_reg_ctrl4(false);
	tx_buff[5] = update_reg_ctrl5(false);
	spi.exchange_sync(spi_config, 6, tx_buff, rx_buff);
}

template < class spi_class >
void L3GD20< spi_class >::enable_irq(irq_t irq_mask, bool write){
	interrupts |= irq_mask;
	if(write)
		update_reg_ctrl3();
}

template < class spi_class >
void L3GD20< spi_class >::disable_irq(irq_t irq_mask, bool write){
	interrupts &= ~irq_mask;
	if(write)
		update_reg_ctrl3();
}

template < class spi_class >
float L3GD20< spi_class >::get_dps_scale(fs_t fs){
	switch(fs){
	case FS_250:
		return (8.75f/1000);
	case FS_500:
		return (17.50f/1000);
	default:
		return (70.0f/1000);
	}
};
