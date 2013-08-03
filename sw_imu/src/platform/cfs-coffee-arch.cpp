#include "ch.h"
#include "hal.h"

#include "platform/cfs-coffee-arch.h"
#include "platform.h"
#include "imu/mt29fxg01.h"

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
static void invert_vect(uint8_t const * src, uint8_t * dst, uint32_t count){
	asm volatile (
		"\tcmp %[count], #32\n" // Compare with 32
		"\tblt lessthanthirtytwo\n"
	"stillgreaterthanthirtytwo:\n"
		"\tldmia %[src]!, {R3-R10}\n"
		"\tmvn R3, R3\n"
		"\tmvn R4, R4\n"
		"\tmvn R5, R5\n"
		"\tmvn R6, R6\n"
		"\tmvn R7, R7\n"
		"\tmvn R8, R8\n"
		"\tmvn R9, R9\n"
		"\tmvn R10, R10\n"
		"\tstmia %[dst]!, {R3-R10}\n"
		"\tsub %[count], #32\n"
		"\tcmp %[count], #32\n" // Compare with 32
		"\tbge stillgreaterthanthirtytwo\n"
	"lessthanthirtytwo:\n"
		"\tcmp %[count], #4\n"
		"\tblt lessthanfour\n"
	"stillgreaterthanfour:\n"
		"\tldmia %[src]!, {R3}\n"
		"\tmvn R3, R3\n"
		"\tstmia %[dst]!, {R3}\n"
		"\tsub %[count], #4\n"
		"\tcmp %[count], #4\n"
		"\tbge stillgreaterthanfour\n"
	"lessthanfour:\n"
		"\tcmp %[count], #0\n"
		"\tbeq doneinverting\n"
		"\tldr r3, [%[src]]\n"                // Load 4 bytes for laughs
		"\tmvn R3, R3\n"                      // Invert that whole word
		"\tcmp %[count], #2\n"
		"\tbeq twoleft\n"
		"\tcmp %[count], #1\n"
		"\tbeq oneleft\n"
		"\tsub %[dst], #1\n"                  // Decrement to allow more
		                                      // efficient addressing
		"\tlsr R4, R3, #8\n"                  // R4 now has 3rd byte LSB
		"\tstrb R4, [%[dst], #1]!\n"
	"twoleft:\n"
		"\tlsr R4, R3, #16\n"                 // R4 now has 2nd byte LSB
		"\tstrb R4, [%[dst], #1]!\n"
	"oneleft:\n"
		"\tlsr R4, R3, #24\n"                 // R4 now has 1st byte LSB
		"\tstrb R4, [%[dst], #1]!\n"
	"doneinverting:\n"
		: 
		: [src] "r" (src),
		  [dst] "r" (dst),
		  [count] "r" (count)
		: "memory", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10"
	);
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
		offset = addr & ((1 << MT29FxG01_OFFSET_BITS) - 1);
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
	uint16_t offset;
	MT29FxG01::page_t * write_buff;
	write_buff = reinterpret_cast<MT29FxG01::page_t *>(chPoolAlloc(&flash.pool));
	// Don't actually write anything in less than a page
	//size -= addr & (flash.page_size - 1);
	//addr = addr & ~(flash.page_size - 1);
	while(size){
		page = (addr >> MT29FxG01_OFFSET_BITS) & (flash.pages_per_block - 1);
		block = addr >> (MT29FxG01_OFFSET_BITS + MT29FxG01_PAGE_BITS);
		offset = addr & ((1 << MT29FxG01_OFFSET_BITS) - 1);
		
		bytes = min(size, flash.page_size - offset);
		
		invert_vect(reinterpret_cast<uint8_t const *>(buf),
		            reinterpret_cast<uint8_t *>(write_buff),
		            bytes);
		
		flash.write(reinterpret_cast<uint8_t const *>(write_buff), block, page,
		            offset, bytes);
		
		buf += bytes;
		addr += bytes;
		size -= bytes;
	}
	chPoolFree(&flash.pool, write_buff);
}


