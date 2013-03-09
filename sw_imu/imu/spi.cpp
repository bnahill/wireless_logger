/*!
 @file 
 
 @brief 
 
 @author Ben Nahill <bnahill@gmail.com>
 */

#include "imu/spi.h"

SPI::SPI(SPIDriver &driver) :
	driver(driver),
	is_init(false)
{
	spiObjectInit(&driver);
	chMBInit(&mb, mb_buffer, mb_size);
}

void SPI::init(){
	//! Check to make sure this is only initialized once
	chSysLock();
	if(!is_init)
		is_init = true;
	else {
		chSysUnlock();
		return;
	}
	chSysUnlock();
	
	thread = chThdCreateStatic(waSPIThread, stack_size, priority, (tfunc_t) start_thread, this);
}

void SPI::transfer(xfer_t &xfer, bool important){
	if(important)
		chMBPostAhead(&mb, (msg_t) &xfer, TIME_INFINITE);
	else
		chMBPost(&mb, (msg_t) &xfer, TIME_INFINITE);
}

void SPI::transferI(xfer_t &xfer, bool important){
	if(important)
		chMBPostAheadI(&mb, (msg_t) &xfer);
	else
		chMBPostI(&mb, (msg_t) &xfer);
}

void SPI::reconfig_clock(){
	
}

void SPI::exchange_sync(slave_config_t const &config, size_t n,
	                    void const * tx_buff, void * rx_buff,
	                    bool important){
	Semaphore done_sem;
	xfer_t xfer(&config, n, tx_buff, rx_buff);
	xfer.tc_sem(&done_sem);
	chSemInit(&done_sem, 0);
	
	transfer(xfer, important);
	
	chSemWait(&done_sem);
}

void SPI::send_sync(slave_config_t const &config, size_t n,
	                void const * tx_buff, bool important){
	Semaphore done_sem;
	xfer_t xfer(&config, n, tx_buff);
	xfer.tc_sem(&done_sem);
	chSemInit(&done_sem, 0);
	
	transfer(xfer, important);
	
	chSemWait(&done_sem);
}

uint16_t SPI::write_sync(slave_config_t const &config, uint16_t addr, size_t n,
	                     void const * tx_buff, bool important){
	Semaphore done_sem;
	xfer_t xfer(&config, n, tx_buff);
	xfer.tc_sem(&done_sem);
	xfer.operation = OP_WRITE;
	xfer.addr = addr;
	chSemInit(&done_sem, 0);
	
	transfer(xfer, important);
	
	chSemWait(&done_sem);
	
	return xfer.addr;
}

uint16_t SPI::read_sync(slave_config_t const &config, uint16_t addr, size_t n,
	                    void * rx_buff, bool important){
	Semaphore done_sem;
	xfer_t xfer(&config, n, OP_READ);
	xfer.tc_sem(&done_sem);
	xfer.addr = addr;
	xfer.rx_buff = rx_buff;
	chSemInit(&done_sem, 0);
	
	transfer(xfer, important);
	
	chSemWait(&done_sem);
	
	return xfer.addr;
}

msg_t SPI::run(){
	xfer_t * xfer;
	is_init = true;
	while(true){
		// Receive a new item to send
		if(RDY_OK != chMBFetch(&mb, (msg_t *) &xfer, TIME_INFINITE))
			continue;
		
		//lock();
		
		spiStart(&driver, xfer->config);
		// Run callback if available
		if(xfer->starting_callback){
			xfer->starting_callback(xfer->starting_param);
		}
		spiSelect(&driver);
		switch(xfer->operation){
		case OP_WRITE:
			spiExchange(&driver, 1, &xfer->addr, &xfer->addr);
			spiSend(&driver, xfer->n, xfer->tx_buff);
			break;
		case OP_READ:
			spiExchange(&driver, 1, &xfer->addr, &xfer->addr);
			spiReceive(&driver, xfer->n, xfer->rx_buff);
			break;
		case OP_EXCHANGE:
			spiExchange(&driver, xfer->n, xfer->tx_buff, xfer->rx_buff);
			break;
		case OP_SEND:
			spiSend(&driver, xfer->n, xfer->tx_buff);
			break;
		}
		spiUnselect(&driver);
		if(xfer->tc_callback){
			xfer->tc_callback(xfer->tc_param);
		}
		spiStop(&driver);
		
		//unlock();
	}
	return 0;
}