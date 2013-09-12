/*!
 @file spi.cpp
 @brief Body for \ref SPI driver
 @author Ben Nahill <bnahill@gmail.com>
 */

#include "imu/spi.h"

SPI::SPI(SPIDriver &driver) :
	driver(driver),
	is_init(false)
{
	//spiObjectInit(&driver);
	//chMBInit(&mb, mb_buffer, mb_size);
}

void SPI::init(){
	//! Check to make sure this is only initialized once
	chSysLock();
	if(!is_init) {
		//thread = chThdCreateI(waSPIThread, stack_size, priority, (tfunc_t) start_thread, this);
		//chThdResumeI(thread);
		is_init = true;
	} 
	chSysUnlock();
}

void SPI::transfer(xfer_t &xfer, bool important){
	lock();
	spiStart(&driver, xfer.config);
	// Run callback if available
	if(xfer.starting_callback){
		xfer.starting_callback(xfer.starting_param);
	}
	spiSelect(&driver);
	clk_mgr_req_hsi();
	switch(xfer.operation){
	case OP_WR_SEQ:
		spiSend(&driver, xfer.n_tx, xfer.tx_buff);
		spiReceive(&driver, xfer.n_rx, xfer.rx_buff);
		break;
	case OP_WW_SEQ:
		spiSend(&driver, xfer.n_tx, xfer.tx_buff);
		spiSend(&driver, xfer.n_rx, xfer.rx_buff);
		break;
	case OP_WRITE:
		spiExchange(&driver, 1, &xfer.addr, &xfer.addr);
		spiSend(&driver, xfer.n_tx, xfer.tx_buff);
		break;
	case OP_READ:
		spiExchange(&driver, 1, &xfer.addr, &xfer.addr);
		spiReceive(&driver, xfer.n_rx, xfer.rx_buff);
		break;
	case OP_EXCHANGE:
		spiExchange(&driver, xfer.n_tx, xfer.tx_buff, xfer.rx_buff);
		break;
	case OP_SEND:
		spiSend(&driver, xfer.n_tx, xfer.tx_buff);
		break;
	}
	clk_mgr_noreq_hsi();
	spiUnselect(&driver);
	spiStop(&driver);
	unlock();
}


void SPI::reconfig_clock(){
	
}

void SPI::exchange_sync(slave_config_t const &config, size_t n,
	                    void const * tx_buff, void * rx_buff,
	                    bool important){
	xfer_t xfer(&config, n, tx_buff, rx_buff);
	transfer(xfer, important);
}

void SPI::send_sync(slave_config_t const &config, size_t n,
	                void const * tx_buff, bool important){
	xfer_t xfer(&config, n, tx_buff);
	transfer(xfer, important);
}

void SPI::wr_sequence_sync(const slave_config_t & config,
                           const uint8_t * tx, uint16_t n_tx,
                           uint8_t * rx, uint16_t n_rx, bool important) {
	xfer_t xfer(&config, n_tx, tx);
	xfer.n_rx = n_rx;
	xfer.rx_buff = rx;
	xfer.operation = OP_WR_SEQ;

	transfer(xfer, important);
}


uint16_t SPI::write_sync(slave_config_t const &config, uint16_t addr, size_t n,
	                     void const * tx_buff, bool important){
	xfer_t xfer(&config, n, tx_buff);
	xfer.operation = OP_WRITE;
	xfer.addr = addr;
	
	transfer(xfer, important);
		
	return xfer.addr;
}

uint16_t SPI::read_sync(slave_config_t const &config, uint16_t addr, size_t n,
	                    void * rx_buff, bool important){
	xfer_t xfer(&config, 0, OP_READ);
	xfer.n_rx = n;
	xfer.addr = addr;
	xfer.rx_buff = rx_buff;
	
	transfer(xfer, important);
	
	return xfer.addr;
}

void SPI::ww_sequence_sync(const slave_config_t & config,
                           const uint8_t * tx0, uint16_t n_tx0,
                           const uint8_t * tx1, uint16_t n_tx1,
                           bool important){
	xfer_t xfer(&config, n_tx0, tx0);
	xfer.n_rx = n_tx1;
	xfer.rx_buff = const_cast<uint8_t *>(tx1);
	xfer.operation = OP_WW_SEQ;
	
	transfer(xfer, important);
}