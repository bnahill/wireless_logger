/*!
 @file cc1101.cpp
 @brief Body for \ref CC1101 class
 @author Ben Nahill <bnahill@gmail.com>
 */

#include "cc1101.h"

CC1101::CC1101(SPI &spi, SPI::slave_config_t slave_config) :
	spi(spi),
	slave_config(slave_config)
{}

bool CC1101::init(){
	spi.init();
	
	strobe(CMD_SRES);
	
	chThdSleep(MS2ST(5));
	
	// Configure synchronous data-out pin so we can read data...
	set_gdo_mode(GDO1, 0x0C);
	
	if(read_reg(REG_FSTEST) != 0x59){
		return false;
	}
	
	//sleep();
	return true;
}


void CC1101::config(reg_config_t const * const initial_config,
                    uint_fast8_t const init_config_len){
	reg_config_t const * iter;
	if(initial_config == nullptr)
		return;
	
	for(iter = initial_config;
		iter < initial_config + init_config_len;
		iter++)
	{
		write_reg(iter->reg, iter->val);
	}
}

void CC1101::early_init(){
	gpio_pin_t ncs(slave_config.ssport, slave_config.sspad);
	
	ncs.clear();
	chThdSleep(1);
	ncs.set();
	chThdSleep(1);
	ncs.clear();
	chThdSleep(1);
	ncs.set();
}

CC1101::status_t CC1101::write_reg(reg_t reg, uint8_t value){
	uint8_t tx_buffer[2];
	status_t rx_buffer[2];
	tx_buffer[0] = reg;
	tx_buffer[1] = value;
	spi.exchange_sync(slave_config, 2, tx_buffer, rx_buffer);
	return rx_buffer[0];
}

uint8_t CC1101::read_reg(reg_t reg){
	uint8_t tx_buffer[2];
	uint8_t rx_buffer[2];
	tx_buffer[0] = reg | MASK_READ;
	tx_buffer[1] = 0;
	spi.exchange_sync(slave_config, 2, tx_buffer, rx_buffer);
	return rx_buffer[1];
}

/*!
	@brief Do a single-word command strobe
	*/
CC1101::status_t CC1101::strobe(reg_t reg){
	uint8_t rx_buffer;
	uint8_t tx_buffer = reg;
	spi.exchange_sync(slave_config, 1, &tx_buffer, &rx_buffer);
	return (status_t)rx_buffer;
}

void CC1101::sleep(){
	strobe(CMD_SPWD);
}

void CC1101::set_gdo_mode(gdo_t gdo, gdo_mode_t gdo_mode){
	write_reg((reg_t)gdo, gdo_mode);
}

CC1101::status_t CC1101::transmit_data(uint_fast8_t const * data, uint_fast8_t len){
	status_t ret;
	ret = spi.write_sync(slave_config, REG_TX_FIFO, len, (void const *)data);
	return ret;
}

CC1101::status_t CC1101::receive_data(uint_fast8_t * dst, uint_fast8_t len){
	status_t ret;
	ret = spi.read_sync(slave_config, REG_RX_FIFO, len, dst);
	return ret;
}