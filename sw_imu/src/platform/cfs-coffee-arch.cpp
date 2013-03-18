#include "platform/cfs-coffee-arch.h"
#include "platform/platform.h"
#include "imu/mt29fxg01.h"
#include <unistd.h>


using namespace Platform;

void fs_flash_erase (coffee_block_t sector){
	flash.erase_block(sector & (FLASH_NUM_BLOCKS - 1));
}

/*!
 @brief A function to bitwise-invert the values of a vector
 @param src The vector to invert
 @param dst The destination
 @param count The number of bytes to invert
 */
extern void inline invert_vect(uint8_t const * src, uint8_t * dst, uint32_t count){
	register union p {
		uint8_t * ptr8;
		uint32_t * ptr32;
		p(uint8_t * val) : ptr8(val){}
	} p1 = const_cast<uint8_t *>(src), p2 = dst;

	while(count >= 32){
		*(p2.ptr32++) = ~*(p1.ptr32++);
		*(p2.ptr32++) = ~*(p1.ptr32++);
		*(p2.ptr32++) = ~*(p1.ptr32++);
		*(p2.ptr32++) = ~*(p1.ptr32++);
		*(p2.ptr32++) = ~*(p1.ptr32++);
		*(p2.ptr32++) = ~*(p1.ptr32++);
		*(p2.ptr32++) = ~*(p1.ptr32++);
		*(p2.ptr32++) = ~*(p1.ptr32++);
		count -= 32;
	}
	while(count >= 4){
		*(p2.ptr32++) = ~*(p1.ptr32++);
		count -= 4;
	}
	while(count--){
		*(p2.ptr8++) = ~*(p1.ptr8++);
	}
}

void fs_flash_read ( coffee_addr_t addr, char * buf, size_t const size ){
	coffee_block_t block;
	coffee_page_t page;
	uint16_t offset;
	uint16_t bytes;
	uint32_t count = size;
	uint8_t * origin = reinterpret_cast<uint8_t *>(buf);
	while(count){
		page = (addr >> MT29FxG01_OFFSET_BITS) & (flash.pages_per_block - 1);
		block = addr >> (MT29FxG01_OFFSET_BITS + MT29FxG01_PAGE_BITS);
		offset = addr & (MT29FxG01_OFFSET_BITS - 1);
		bytes = min(count, flash.page_size - offset);
		
		flash.read_page(reinterpret_cast<uint8_t *>(buf), block, page, offset, bytes);
		
		buf += bytes;
		addr += bytes;
		count -= bytes;
	}
	// Adjust for the fact that coffee expects the erase values to actually be 0...
	invert_vect(origin, origin, size);
}

void fs_flash_write ( coffee_addr_t addr, char const * buf, size_t size ){
	coffee_block_t block;
	coffee_page_t page;
	uint16_t bytes;
	MT29FxG01::page_t * write_buff;
	write_buff = reinterpret_cast<MT29FxG01::page_t *>(chPoolAlloc(&flash.pool));
	// Don't actually write anything in less than a page
	//size -= addr & (flash.page_size - 1);
	//addr = addr & ~(flash.page_size - 1);
	while(size){
		page = (addr >> MT29FxG01_OFFSET_BITS) & (flash.pages_per_block - 1);
		block = addr >> (MT29FxG01_OFFSET_BITS + MT29FxG01_PAGE_BITS);
		
		bytes = min(size, sizeof(*write_buff));
		
		invert_vect(reinterpret_cast<uint8_t const *>(buf),
		            reinterpret_cast<uint8_t *>(write_buff),
		            size);
		
		flash.write_page(reinterpret_cast<uint8_t const *>(write_buff), block, page);
		
		buf += bytes;
		addr += bytes;
		size -= bytes;
	}
	chPoolFree(&flash.pool, write_buff);
}


