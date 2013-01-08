#ifndef __IMU_SPI_H_
#define __IMU_SPI_H_

#include "ch.h"
#include "hal.h"

class SPI_Transfer;

class SPI {
public:
	typedef SPIConfig slave_config_t;
	
	/*
	 * These additions are for a potential move to asynchronous operations
	 * where a gatekeepter thread would exist for each SPI device and a FIFO
	 * would be used to serialize operations. At the moment this is
	 * unnecessary.
	 */
	typedef enum {
		OP_READ,
		OP_WRITE,
		OP_EXCHANGE,
		OP_SEND
	} operation_t;
	
	typedef void (*callback)(void *);


	struct xfer_t {
		//! Constructor for synchronous exchange
		xfer_t(slave_config_t *config, size_t n, void const * tx_buff, void * rx_buff) :
			config(config), n(n),
			tc_callback(nullptr), starting_callback(nullptr),
			operation(OP_EXCHANGE), rx_buff(rx_buff), tx_buff(tx_buff)
		{}
		
		//! Constructor for send
		xfer_t(slave_config_t *config, size_t n, void const * tx_buff) :
			config(config), n(n),
			tc_callback(nullptr), starting_callback(nullptr),
			operation(OP_SEND), tx_buff(tx_buff)
		{}

		//xfer_t(slave_config_t *config, operation_t operation, size_t n, void * buff, uint16_t addr){
		//	config(config),
		//	tc_callback(nullptr), starting_callback(nullptr), para
		//}
		//! Slave information
		slave_config_t * config;
		//! @name Callbacks
		//! @brief Set to 0 to not use
		//! @{
		//! Callback for when the transfer is completely done
		callback tc_callback;
		//! Callback for when the transfer is just about to start
		callback starting_callback;
		void *tc_param;
		void *starting_param;
		//! @}
		//! @name Transfer details
		//! @{
		//! The operation to perform
		operation_t operation;
		//! Number of bytes to send, receive, or exchange
		size_t n;
		//! Buffer to write
		void const * tx_buff;
		//! Buffer to store received information
		void * rx_buff;
		//! Address for read or write transfers
		uint16_t addr;
		//! @}
		
		void tc_sem(Semaphore *sem){
			tc_callback = (callback) callback_semaphore;
			tc_param = sem;
		}
	};
	
	SPI(SPIDriver &driver) :
		driver(driver),
		is_init(false)
	{
		spiObjectInit(&driver);
		chMBInit(&mb, mb_buffer, mb_size);
	}
	
	void init(){
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
	
	void transfer(xfer_t &xfer, bool important=false){
		if(important)
			chMBPostAhead(&mb, (msg_t) &xfer, TIME_INFINITE);
		else
			chMBPost(&mb, (msg_t) &xfer, TIME_INFINITE);
	}
	
	/*!
	 @brief Perform a synchronous exchange with a peripheral
	 @param config The slave configuration to use
	 @param n The number of bytes to exchange
	 @param tx_buff The n-length buffer to transmit
	 @param rx_buff The n-length receive buffer
	 @param important Force to front of queue?
	 */
	void exchange_sync(slave_config_t &config, size_t n, void const * tx_buff, void * rx_buff, bool important=false){
		Semaphore done_sem;
		xfer_t xfer(&config, n, tx_buff, rx_buff);
		xfer.tc_sem(&done_sem);
		chSemInit(&done_sem, 0);
		
		transfer(xfer, important);
		
		chSemWait(&done_sem);
	}
	
	/*!
	 @brief Perform a synchronous transmission
	 @param config The slave configuration to use
	 @param n The number of bytes to send
	 @param tx_buff The n-length buffer to transmit
	 @param important Force to front of queue?
	 */
	void send_sync(slave_config_t &config, size_t n, void const * tx_buff, bool important=false){
		Semaphore done_sem;
		xfer_t xfer(&config, n, tx_buff);
		xfer.tc_sem(&done_sem);
		chSemInit(&done_sem, 0);
		
		transfer(xfer, important);
		
		chSemWait(&done_sem);
	}
	
	//void acquire(){spiAcquireBus(&driver);}
	//void release(){spiReleaseBus(&driver);}
	
protected:
	//! The ChibiOS SPI resource
	SPIDriver &driver;
	
	/*!
	 @brief Flag to indicate that it shouldn't be re-initialized.
	 @note This variable should be accessed only under lock.
	 */
	bool is_init;
	
	msg_t run(){
		xfer_t const * xfer;
		is_init = true;
		while(true){
			// Receive a new item to send
			if(RDY_OK != chMBFetch(&mb, (msg_t *) &xfer, TIME_INFINITE))
				continue;
			
			spiStart(&driver, xfer->config);
			// Run callback if available
			if(xfer->starting_callback){
				xfer->starting_callback(xfer->starting_param);
			}
			spiSelect(&driver);
			switch(xfer->operation){
			case OP_WRITE:
				spiSend(&driver, 1, &xfer->addr);
				spiSend(&driver, xfer->n, xfer->tx_buff);
				break;
			case OP_READ:
				spiSend(&driver, 1, &xfer->addr);
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
		}
	}
	
	static constexpr uint32_t stack_size = 1024;
	static constexpr tprio_t priority = HIGHPRIO;
	static constexpr tprio_t mb_size = 16;
	
	WORKING_AREA(waSPIThread, stack_size);
	Thread *thread;
	Mailbox mb;
	msg_t mb_buffer[mb_size];
	
	static msg_t start_thread(SPI *spi){
		spi->run();
	}
	
	/*!
	 @brief A callback for simply signalling a semaphore
	 */
	static void callback_semaphore(Semaphore *sem){
		chSemSignal(sem);
	}
};



#endif // __IMU_SPI_H_
