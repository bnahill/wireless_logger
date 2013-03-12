/*!
 @file mt29fxg01.h
 @brief Declaration for \ref MT29FxG01 class
 @author Ben Nahill <bnahill@gmail.com>
 */

#ifndef __IMU_MT29FXG01_H_
#define __IMU_MT29FXG01_H_

#include "imu/imu.h"
#include "imu/spi.h"

class MT29FxG01 {
public:
	typedef enum {
		SIZE_1G = 1024,
		SIZE_2G = 2048,
		SIZE_4G = 4096
	} numblocks_t;

	typedef enum {
		CMD_BLOCK_ERASE            = 0xD8,
		CMD_GET_FEATURE            = 0x0F,
		CMD_PAGE_READ              = 0x13,
		CMD_PROGRAM_EXECUTE        = 0x10,
		CMD_PROGRAM_LOAD           = 0x02,
		CMD_PROGRAM_LOAD_RANDOM    = 0x84,
		CMD_READ_CACHE             = 0x03, // 0x0B
		CMD_READ_CACHE_X2          = 0x3B,
		CMD_READ_CACHE_X4          = 0x6B,
		CMD_READ_ID                = 0x9F,
		CMD_RESET                  = 0xFF,
		CMD_SET_FEATURE            = 0x1F,
		CMD_WRITE_DISABLE          = 0x04,
		CMD_WRITE_ENABLE           = 0x06
	} mt29fxg01_cmd_t;
	
	MT29FxG01(SPI &spi, numblocks_t num_blocks,
	          gpio_pin_t const &ncs,
	          gpio_pin_t const &nwp,
	          gpio_pin_t const &nhold,
	          uint16_t spi_flags);
	
	/*!
	 @brief Initialize the device
	 @return Success
	 @pre The scheduler is running
	 */
	bool init();
	
	bool read_page(uint8_t * dst, uint16_t block, uint8_t page,
	               uint16_t offset, uint16_t bytes);
	bool write_page(uint8_t const * src, uint16_t block, uint8_t page,
	                uint16_t offset, uint16_t bytes);
	bool erase_block(uint16_t block);
protected:
	typedef enum {
		FADDR_BLOCK_LOCK = 0xA0,
		FADDR_OTP        = 0xB0,
		FADDR_STATUS     = 0xC0
	} feature_addr_t;
	
	typedef enum {
		BLMASK_BRWD = 0x80,
		BLMASK_BP2  = 0x20,
		BLMASK_BP1  = 0x10,
		BLMASK_BP0  = 0x08
	} block_lock_mask_t;
	
	typedef enum {
		OTPMASK_PROTECT = 0x80,
		OTPMASK_ENABLE  = 0x40,
		OTPMASK_ECC_EN  = 0x10
	} otp_mask_t;
	
	typedef enum {
		STATMASK_ECC_STAT1 = 0x20,
		STATMASK_ECC_STAT0 = 0x10,
		STATMASK_P_FAIL    = 0x08,
		STATMASK_E_FAIL    = 0x04,
		STATMASK_WEL       = 0x02,
		STATMASK_OIP       = 0x01
	} status_mask_t;
	
	void set_feature(feature_addr_t feature, uint8_t value);
	uint8_t get_feature(feature_addr_t feature);
	
	void reset();
	
	void write_enable();
	
	SPI &spi;
	SPI::slave_config_t const spi_slave;
	gpio_pin_t nwp, ncs, nhold;
	
	uint8_t inline col_addr_high(uint16_t block, uint16_t offset){
		return (offset >> 8) | ((block & 1) << 4);
	}
	
	uint8_t inline col_addr_low(uint16_t block, uint16_t offset){
		return offset;
	}
	
	void lock(){chMtxLock(&mutex);}
	void unlock(){chMtxUnlock();}
	
	Mutex mutex;
	
	// Parameters from the datasheet --  should hold for many different flash
	// sizes
	static constexpr uint32_t page_size = 2048; //!< Number of bytes in page
	static constexpr uint32_t page_spare = 64; //!< Number of spare bytes in a page
	static constexpr uint32_t pages_per_block = 64; //!< Number of pages in a block
	
	// This deterimines what size device it is
	uint32_t const num_blocks;
};

#endif // __IMU_MT29FXX_H_
