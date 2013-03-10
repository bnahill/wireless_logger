/*!
 @file spi.h
 @brief Declaration for \ref SPI driver
 @author Ben Nahill <bnahill@gmail.com>
 */

#ifndef __IMU_SPI_H_
#define __IMU_SPI_H_

#include "ch.h"
#include "hal.h"
#include "imu/imu.h"

//! @addtogroup IMU
//! @{
//! @addtogroup SPI
//! @{

//class SPI : public LockableSem {
/*!
 @brief A 4-wire SPI driver
 */
class SPI {
public:
	/*
	typedef struct {
		gpio_pin_t nss;
		uint32_t max_freq;
		uint32_t cr1_flags;
	} slave_config_t;
	*/
	
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
		xfer_t(slave_config_t const * config, size_t n, void const * tx_buff, void * rx_buff) :
			config(config), n(n),
			tc_callback(nullptr), starting_callback(nullptr),
			operation(OP_EXCHANGE), rx_buff(rx_buff), tx_buff(tx_buff)
		{}
		
		//! Constructor for send
		xfer_t(slave_config_t const * config, size_t n, void const * tx_buff) :
			config(config), n(n),
			tc_callback(nullptr), starting_callback(nullptr),
			operation(OP_SEND), tx_buff(tx_buff)
		{}
		
		//! A more generic constructor
		xfer_t(slave_config_t const * config, size_t n, operation_t op) :
			config(config), n(n), operation(op),
			tc_callback(nullptr), starting_callback(nullptr)
		{}

		//! Slave information
		slave_config_t const * config;
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
	
	SPI(SPIDriver &driver);
	
	void init();
	
	/*!
	 @brief Add a transfer to the transfer queue
	 @param xfer The transfer to add
	 @param important Push to front of queue?
	 */
	void transfer(xfer_t &xfer, bool important=false);
	
	/*!
	 @brief Add a transfer to the transfer queue (from lock context)
	 @param xfer The transfer to add
	 @param important Push to front of queue?
	 */
	void transferI(xfer_t &xfer, bool important=false);
	
	void reconfig_clock();
	
	/*!
	 @brief Perform a synchronous exchange with a peripheral
	 @param config The slave configuration to use
	 @param n The number of bytes to exchange
	 @param tx_buff The n-length buffer to transmit
	 @param rx_buff The n-length receive buffer
	 @param important Force to front of queue?
	 */
	void exchange_sync(slave_config_t const &config, size_t n,
	                   void const * tx_buff, void * rx_buff,
	                   bool important=false);
	
	/*!
	 @brief Perform a synchronous transmission
	 @param config The slave configuration to use
	 @param n The number of bytes to send
	 @param tx_buff The n-length buffer to transmit
	 @param important Force to front of queue?
	 */
	void send_sync(slave_config_t const &config, size_t n,
	               void const * tx_buff, bool important=false);
	
	/*!
	 @brief Perform a synchronous transmission
	 @param config The slave configuration to use
	 @param addr The address to write to
	 @param n The number of bytes to send
	 @param tx_buff The n-length buffer to transmit
	 @param important Force to front of queue?
	 @return The value read during address transmission
	 */
	uint16_t write_sync(slave_config_t const &config, uint16_t addr, size_t n,
	                void const * tx_buff, bool important=false);
	
	/*!
	 @brief Perform a synchronous transmission
	 @param config The slave configuration to use
	 @param addr The address to write to
	 @param n The number of bytes to send
	 @param tx_buff The n-length buffer to transmit
	 @param important Force to front of queue?
	 @return The value read during address transmission
	 */
	uint16_t read_sync(slave_config_t const &config, uint16_t addr, size_t n,
	                void * rx_buff, bool important=false);
	
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
	
	bool do_sleep;
	
	msg_t run();
	
	static constexpr uint32_t stack_size = 1024;
	static constexpr tprio_t priority = HIGHPRIO;
	static constexpr tprio_t mb_size = 16;
	
	WORKING_AREA(waSPIThread, stack_size);
	Thread *thread;
	Mailbox mb;
	msg_t mb_buffer[mb_size];
	
	static msg_t start_thread(SPI * spi){
		spi->run();
		return 0;
	}
	
	/*!
	 @brief A callback for simply signalling a semaphore
	 */
	static void callback_semaphore(Semaphore *sem){
		chSemSignal(sem);
	}
};


//! @} @}

#endif // __IMU_SPI_H_
