#ifndef __PLATFORM_CFS_COFFEE_ARCH_H_
#define __PLATFORM_CFS_COFFEE_ARCH_H_

#include "imu/mt29fxg01.h"
#include "stdint.h"

//! The block index
typedef uint16_t coffee_block_t;
//! The address of a single page
typedef uint16_t coffee_page_t;
//! A complete byte address
typedef uint32_t coffee_addr_t;


#ifdef __cplusplus
extern "C" {
#endif
	void fs_flash_erase(coffee_block_t sector);
	void fs_flash_read(coffee_addr_t offset, char * buf, size_t size);
	void fs_flash_write(coffee_addr_t offset, char const * buf, size_t size);
#ifdef __cplusplus
}
#endif

// This is a 1Gb unit
#define FLASH_BLOCK_BITS        (10)
#define FLASH_NUM_BLOCKS        (1 << FLASH_BLOCK_BITS)

#define COFFEE_SECTOR_SIZE      (MT29FxG01_PAGES_PER_BLOCK * MT29FxG01_PAGE_SIZE)
#define COFFEE_PAGE_SIZE        (MT29FxG01_PAGE_SIZE)
#define COFFEE_START            0
#define COFFEE_SIZE             (COFFEE_SECTOR_SIZE * FLASH_NUM_BLOCKS)
#define COFFEE_NAME_LENGTH      16
#define COFFEE_MAX_OPEN_FILES   6
#define COFFEE_FD_SET_SIZE      8
#define COFFEE_LOG_TABLE_LIMIT  256
#define COFFEE_DYN_SIZE         32*1024UL
#define COFFEE_LOG_SIZE         8*1024UL
#define COFFEE_MICRO_LOGS       1
// If a file is created which follows flash page boundaries, this is good
#define COFFEE_IO_SEMANTICS     1
// Some sort of better wear leveling
#define COFFEE_EXTENDED_WEAR_LEVELLING 1

#define COFFEE_WRITE(buf, size, offset)				\
		fs_flash_write(COFFEE_START + (offset), (char *)(buf), (size))

#define COFFEE_READ(buf, size, offset)				\
  		fs_flash_read(COFFEE_START + (offset), (char *)buf, (size))

#define COFFEE_ERASE(sector)					\
		fs_flash_erase(sector)

#endif // __PLATFORM_CFS_COFFEE_ARCH_H_
