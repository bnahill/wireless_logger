#include "platform/cfs-coffee-arch.h"
#include "platform/platform.h"
#include "imu/mt29fxg01.h"


using namespace Platform;

void fs_flash_erase (coffee_block_t sector){
	flash.erase_block(sector & (FLASH_NUM_BLOCKS - 1));
}

void fs_flash_read ( coffee_addr_t addr, char * buf, size_t size ){
	coffee_block_t block;
	coffee_page_t page;
	uint16_t offset;
	uint16_t bytes;
	while(size){
		page = (addr >> MT29FxG01_OFFSET_BITS) & (flash.pages_per_block - 1);
		block = addr >> (MT29FxG01_OFFSET_BITS + MT29FxG01_PAGE_BITS);
		offset = addr & (MT29FxG01_OFFSET_BITS - 1);
		bytes = flash.page_size - offset;
		
		flash.read_page(reinterpret_cast<uint8_t *>(buf), block, page, offset, bytes);
		
		buf += bytes;
		addr += bytes;
		size -= bytes;
	}
}

void fs_flash_write ( coffee_addr_t addr, char const * buf, size_t size ){
	coffee_block_t block;
	coffee_page_t page;
	uint16_t bytes;
	// Don't actually write anything in less than a page
	size -= addr & (flash.page_size - 1);
	addr = addr & ~(flash.page_size - 1);
	while(size){
		page = (addr >> MT29FxG01_OFFSET_BITS) & (flash.pages_per_block - 1);
		block = addr >> (MT29FxG01_OFFSET_BITS + MT29FxG01_PAGE_BITS);
		
		bytes = flash.page_size;
		
		flash.write_page(reinterpret_cast<uint8_t const *>(buf), block, page);
		
		buf += bytes;
		addr += bytes;
		size -= bytes;
	}
}


