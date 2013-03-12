/*!
 @file mt29fxg01.cpp
 @brief Body for \ref MT29FxG01 class
 @author Ben Nahill <bnahill@gmail.com>
 */

#include "imu/mt29fxg01.h"

MT29FxG01::MT29FxG01(SPI &spi,
                     numblocks_t num_blocks,
                     gpio_pin_t const &ncs,
                     gpio_pin_t const &nwp,
                     gpio_pin_t const &nhold,
                     uint16_t spi_flags) :
	spi(spi), nwp(nwp), ncs(ncs), nhold(nhold), num_blocks(num_blocks),
	spi_slave({nullptr, &ncs.gpio, ncs.index, spi_flags})
{
	chMtxInit(&mutex);
}

void MT29FxG01::write_enable(){
	uint8_t x = CMD_WRITE_ENABLE;
	spi.send_sync(spi_slave, 1, &x);
}


bool MT29FxG01::init(){
	uint8_t rx[4] = {0};
	uint8_t tx[4] = {CMD_READ_ID, 0,0,0};
	
	nhold.set();
	nwp.set();
	
	spi.init();
	
	reset();
	
	chThdSleep(MS2ST(1));
	
	spi.exchange_sync(spi_slave, 4, static_cast<void*>(tx), rx);
	return (rx[2] == 0x2C) && (rx[3] == 0x12);
}

void MT29FxG01::reset(){
	uint8_t tx = CMD_RESET;
	spi.send_sync(spi_slave, 1, &tx);
}

uint8_t MT29FxG01::get_feature(feature_addr_t feature){
	uint8_t rx[3];
	uint8_t tx[3] = {
		CMD_GET_FEATURE,
		feature,
		0
	};
	spi.exchange_sync(spi_slave, 3, tx, rx);
	return rx[2];
}

void MT29FxG01::set_feature(feature_addr_t feature, uint8_t value){
	uint8_t tx[3] = {
		CMD_SET_FEATURE,
		feature,
		value
	};
	// Clear bits 6 and 7 if it might mess with the OTP area
	if(feature == FADDR_OTP)
		tx[2] &= 0x3F;
	spi.send_sync(spi_slave, 3, tx);
}

bool MT29FxG01::read_page(uint8_t * dst, uint16_t block, uint8_t page, uint16_t offset, uint16_t bytes){
	uint8_t tx[4];
	uint8_t stat;
	constexpr uint32_t num_tries = 100;
	uint32_t i;
	tx[0] = CMD_PAGE_READ;
	tx[1] = block >> 10;
	tx[2] = block >> 2;
	tx[3] = (block << 6) | page;
	
	lock();
	spi.send_sync(spi_slave, 4, tx);
	for(i = 0; i < num_tries; i++){
		chThdYield();
		stat = get_feature(FADDR_STATUS);
		if((stat & STATMASK_OIP) == 0){
			if(stat & STATMASK_ECC_STAT1){
				// There is an uncorrected error here
				unlock();
				return false;
			}
			break;
		}
	} if(i == num_tries) {
		unlock();
		return false;
	}
	
	// Now read that page from cache
	
	tx[0] = CMD_READ_CACHE;
	tx[1] = col_addr_high(block,offset);
	tx[2] = col_addr_low(block,offset);
	tx[3] = 0;
	
	spi.wr_sequence_sync(spi_slave, tx, 4, dst, bytes);
	
	unlock();
	
	return true;
}

bool MT29FxG01::write_page(uint8_t const * src, uint16_t block,
                           uint8_t page, uint16_t offset, uint16_t bytes){
	uint8_t tx[4];
	uint8_t stat;
	constexpr uint32_t num_tries = 200;
	uint32_t i;
	lock();
	write_enable();
	tx[0] = CMD_PROGRAM_LOAD_RANDOM;
	tx[1] = col_addr_high(block, offset);
	tx[2] = col_addr_low(block, offset);
	spi.ww_sequence_sync(spi_slave, tx, 3, src, bytes);
	
	tx[0] = CMD_PROGRAM_EXECUTE;
	spi.send_sync(spi_slave, 1, tx);
	
	for(i = 0; i < num_tries; i++){
		chThdYield();
		stat = get_feature(FADDR_STATUS);
		if((stat & STATMASK_OIP) == 0){
			// Done
			unlock();
			return true;
		}
	}
	
	unlock();
	return false;
}

bool MT29FxG01::erase_block ( uint16_t block ){
	uint8_t tx[4];
	uint8_t stat;
	constexpr uint32_t num_tries = 5;
	uint32_t i;
	
	lock();
	write_enable();
	tx[0] = CMD_BLOCK_ERASE;
	tx[1] = block >> 10;
	tx[2] = block >> 2;
	tx[3] = (block << 6);
	
	for(i = 0; i < num_tries; i++){
		chThdSleep(MS2ST(1));
		stat = get_feature(FADDR_STATUS);
		if((stat & STATMASK_OIP) == 0){
			// Done
			unlock();
			return true;
		}
	}
	
	unlock();
	return false;
}
