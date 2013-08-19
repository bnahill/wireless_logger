/*!
 @file mt29fxg01.h
 @brief Declaration for \ref MT29FxG01 class
 @author Ben Nahill <bnahill@gmail.com>
 */

#ifndef __IMU_MT29FXG01_H_
#define __IMU_MT29FXG01_H_

#define MT29FxG01_PAGE_BITS       (6)
#define MT29FxG01_OFFSET_BITS     (11)

#define MT29FxG01_PAGE_SIZE            (1 << MT29FxG01_OFFSET_BITS)
#define MT29FxG01_PAGE_SPARE           (64)
#define MT29FxG01_PAGE_TOTAL_PAGE_SIZE (MT29FxG01_PAGE_SIZE + MT29FxG01_PAGE_SPARE)
#define MT29FxG01_PAGES_PER_BLOCK      (1 << MT29FxG01_PAGE_BITS)


#if defined(IS_DOXYGEN) || defined(__cplusplus)

#include "imu/imu.h"
#include "imu/spi.h"
#include "ch.h"
#include "hal.h"

/*!
 @brief A driver for Micron MT29FxG01 @ref SPI flash memory
 */
class MT29FxG01 {
public:
	//! The number of blocks in each variant of the chip
	typedef enum {
		SIZE_1G = 1024,
		SIZE_2G = 2048,
		SIZE_4G = 4096
	} numblocks_t;

	//! Supported commands
	typedef enum {
		CMD_BLOCK_ERASE            = 0xD8, //!< Write a block
		CMD_GET_FEATURE            = 0x0F, //!< Read a feature register
		CMD_PAGE_READ              = 0x13, //!< Read a page to cache
		CMD_PROGRAM_EXECUTE        = 0x10, //!< Execute a write
		CMD_PROGRAM_LOAD           = 0x02, //!< Clear then load a page to cache
		CMD_PROGRAM_LOAD_RANDOM    = 0x84, //!< Load a page to cache
		CMD_READ_CACHE             = 0x03, //!< Read a page from cache
		CMD_READ_CACHE_X2          = 0x3B,
		CMD_READ_CACHE_X4          = 0x6B,
		CMD_READ_ID                = 0x9F, //!< Read the device ID
		CMD_RESET                  = 0xFF, //!< Reset device configuration
		CMD_SET_FEATURE            = 0x1F, //!< Set a feature register
		CMD_WRITE_DISABLE          = 0x04, //!< Disable writes
		CMD_WRITE_ENABLE           = 0x06  //!< Enable writes
	} cmd_t;
	
	MT29FxG01(SPI &spi, numblocks_t num_blocks,
	          gpio_pin_t const &ncs,
	          uint16_t spi_flags);
	
	/*!
	 @name Datasheet parameters
	 @brief Parameters from the datasheet --  should hold for many different
	 flash sizes
	 @{
	 */
	//! Number of bytes in page
	static constexpr uint32_t page_size = MT29FxG01_PAGE_SIZE;
	 //! Number of spare bytes in a page
	static constexpr uint32_t page_spare = MT29FxG01_PAGE_SPARE;
	static constexpr uint32_t total_page_size = page_size + page_spare;
	//! Number of pages in a block
	static constexpr uint32_t pages_per_block = MT29FxG01_PAGES_PER_BLOCK;
	//! @}
	
	//! This deterimines what size device it is
	uint32_t const num_blocks;
	
	//! A buffer for a single page
	typedef uint8_t page_t[page_size];
	
	/*!
	 @brief Reset device and set basic configuration
	 */
	void reset();
	
	/*!
	 @brief Initialize the device
	 @return Success
	 @pre The scheduler is running
	 */
	bool init();
	
	bool read_page(uint8_t * dst, uint16_t block, uint8_t page,
	               uint16_t offset, uint16_t bytes);
	bool write_page(uint8_t const * src, uint16_t block, uint8_t page);
	bool write(uint8_t const * src, uint16_t block, uint8_t page,
	           uint16_t offset, uint16_t n);
	bool inline write_page(page_t const &src, uint16_t block, uint8_t page){
		return write_page(static_cast<uint8_t const *>(src), block, page);
	}
	bool erase_block(uint16_t block);
	bool check_block_bad(uint16_t block);
	
	/*!
	 @brief Open a page for a sequence of edits
	 
	 This will read a page to a buffer in flash to be modified
	 
	 @note This MUST be followed by a call to \ref page_commit in the same thread
	 */
	bool page_open(uint16_t block, uint8_t page);

	bool page_open_write(uint16_t block, uint8_t page);

	/*!
	 @brief Write to an open page from \ref page_open
	 
	 This will not be committed until a cal to \ref page_commit
	 */
	bool page_write_continued(uint8_t const * src, uint16_t offset, uint16_t n);
	/*!
	 @brief Read from an open page from \ref page_open
	 */
	bool page_read_continued(uint8_t * dst, uint16_t offset, uint16_t n);
	/*!
	 @brief Commit a modified page back to the flash bank
	 
	 @pre \ref page_open was called
	 */
	bool page_commit();
	
	/*!
	 @brief Go read all blocks and rewrite them completely to refresh the data
	 
	 Occasional use of this function should reduce the frequency of bit errors
	 */
	void cleanup();
	
	//! Lock (in mutual exclusion sense) the device
	void lock(){chMtxLock(&mutex);}
	//! Unlock (in mutual exclusion sense) the device
	void unlock(){chMtxUnlock();}
	
	
	MemoryPool pool;
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
	
	uint8_t writes_so_far;

	
	/*!
	 @brief Set required configuration (following reset)
	 */
	void set_defaults();
	
	/*!
	 @brief Set the WEL bit in the device to allow writes and erases
	 */
	void write_enable();
	
	//! The SPI device to use
	SPI &spi;
	//! The SPI configuration to use
	SPI::slave_config_t const spi_slave;
	
	//! GPIO pins required
	gpio_pin_t ncs;
	
	//! An address in flash
	struct address_t {
		/*
		 * 10-bit block address
		 * 6-bit in-block page address
		 * 11-bit in-page address
		 */
		address_t(uint16_t block, uint16_t page, uint16_t offset) :
			block(block), page(page & (pages_per_block - 1)),
			offset(offset)
		{}
		
		address_t() : 
		address_t(0,0,0){};
		
		uint16_t block;
		uint16_t page;
		uint16_t offset;
		void get_column_address(uint8_t * dst) const {
			dst[0] = ((block & 1) << 4) | (offset >> 8);
			dst[1] = offset & 0xFF;
		}
		
		void get_row_address(uint8_t * dst) const {
			dst[0] = block >> 10;
			dst[1] = block >> 2;
			dst[2] = (block << 6) | page;
		}
	};
	
	address_t current_addr;
	
	void page_read_to_cache(address_t const &addr);
	
	/*!
	 @brief Load a page of data into the cache
	 @param addr The page address
	 @param src The data to write
	 @param n The number of bytes to program
	 @param random Is there existing data to preserve?
	 */
	void program_load(address_t const &addr, uint8_t const * src,
	                  uint16_t n, bool random = false);
	
	/*!
	 @brief Execute a write based on data loaded with @ref program_load
	 @param addr The address to write to
	 @note This doesn't wait for the operation to finish
	 */
	void program_execute(address_t const &addr);
	
	Mutex mutex;
	
	//! Number of page buffers available for allocation
	static constexpr uint32_t num_page_buffers = 2;
	
	//! A buffer pool for readers and writers
	page_t pool_buffer[num_page_buffers];
};

#endif // __cplusplus

#endif // __IMU_MT29FXX_H_
