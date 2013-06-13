#ifdef __FLOGFS_CONF_H_
#error "Only one file should include flogfs_conf.h. Something is wrong"
#endif

#define __FLOGFS_CONF_H_

#include "flogfs.h"

#include "platform/platform.h"

using namespace Platform;

// Flash module parameters
#define FS_CHUNK_SIZE       (512)
#define FS_CHUNKS_PER_PAGE  (4)
#define FS_PAGES_PER_BLOCK  (64)
#define FS_NUM_BLOCKS       (1024)

// Filesystem options

//! With this enabled, bad blocks will not be erased and will instead be left
//! to rot.
#define FORMAT_PRESERVE_BAD_BLOCKS   (1)

//! Erase discarded blocks if their sequence number is relatively really low
#define ERASE_EXPIRED                (1)
//! When a discarded block has been left for this amount of time, it will be
//! erased at startup
#define ERASE_EXPIRATION_TIME        (FS_NUM_BLOCKS * 3 / 4)

typedef uint8_t flash_page_md_t[59];

typedef uint8_t fs_lock_t;

uint8_t flogfs_hamming_distance8_default(uint8_t a, uint8_t b);

/*!
 @brief Comparing two bytes and identify the number of bit flips between them
 @param a One byte
 @param b The other byte
 @return The number of bit flips
 
 Some data has to be stored in areas with no ECC protection. This data must
 still be preserved so it is assumed that a single bit error may appear
 in the byte. Therefore, we need to search for the nearest valid result
 */
static inline uint8_t flogfs_hamming_distance8(uint8_t a, uint8_t b){
	// Use the reference implementation
	return flogfs_hamming_distance8_default(a,b);
}

static inline void fs_lock(fs_lock_t * lock){
	
}

static inline void fs_unlock(fs_lock_t * lock){
	
}

static flash_page_md_t md_buffer;
static uint16_t flash_block;
static uint16_t flash_page;
static uint8_t have_metadata;
static uint8_t page_open;

static inline flog_result_t flash_init(){
	page_open = 0;
	have_metadata = 0;
	return FLOG_RESULT(flash.init());
}

static inline void flash_lock(){
	flash.lock();
}

static inline void flash_unlock(){
	flash.unlock();
}

static inline flog_result_t flash_open_page(uint16_t block, uint16_t page){
	flash_block = block;
	flash_page = page;
	page_open = 1;
	have_metadata = 0;
	
	// Read flash page to cache
	return FLOG_RESULT(flash.page_open(block, page));
}

static inline void flash_close_page(){
	flash.unlock();
}

static inline flog_result_t flash_erase_block(uint16_t block){
	return FLOG_RESULT(flash.erase_block(block));
}

static inline flog_result_t flash_get_metadata(){
	// Read metadata from flash
	if(!have_metadata){
		have_metadata = 1;
	}
	return FLOG_RESULT(flash.page_read_continued(md_buffer, 0x800, sizeof(md_buffer)));
}

static inline uint8_t * flash_meta_protected(uint8_t chunk_in_page){
	return &md_buffer[chunk_in_page * 16 + 4];
}

static inline uint8_t * flash_meta_unprotected(uint8_t chunk_in_page){
	return &md_buffer[chunk_in_page * 16 + 2];
}

static inline flog_result_t flash_block_is_bad(){
	if(!have_metadata){
		flash_get_metadata();
	}
	return FLOG_RESULT(md_buffer[0] == 0);
}

static inline void flash_set_bad_block(){
	
}

/*!
 @brief Commit the changes to the active page
 */
static inline void flash_commit(){
	page_open = 0;
	flash.page_commit();
}

/*!
 @brief Read data from the flash cache (current page only)
 @param dst The destination buffer to fill
 @param chunk_in_page The chunk index within the current page
 @param offset The offset data to retrieve
 @param n The number of bytes to transfer
 @return The success or failure of the operation
 */
static inline flog_result_t flash_read_chunk(uint8_t * dst, uint8_t chunk_in_page, uint16_t offset, uint16_t n){
	return FLOG_RESULT(flash.page_read_continued(dst, FS_CHUNK_SIZE * chunk_in_page + offset, n));
}

/*!
 @brief Write chunk data to the flash cache
 @param src A pointer to the data to transfer
 @param chunk_in_page The chunk index within the current page
 @param offset The offset to write the data
 @param n The number of bytes to write
 */
static inline void flash_write_chunk(uint8_t const * src, uint8_t chunk_in_page, uint16_t offset, uint16_t n){
	flash.page_write_continued(src, FS_CHUNK_SIZE * chunk_in_page + offset, n);
}


/*!
 @brief Write the MD2 field
 @param chunk_in_page The chunk index within the current page
 
 @note This doesn't commit the transaction
 */
static inline void flash_write_meta_protected(uint8_t chunk_in_page){
	flash.page_write_continued(flash_meta_protected(chunk_in_page),
	    2048 + (flash_meta_protected(chunk_in_page) - md_buffer), 4);
}

/*!
 @brief Write the MD2 field
 @param chunk_in_page The chunk index within the current page
 */
static inline void flash_write_meta_unprotected(uint8_t chunk_in_page){
	flash.page_write_continued(flash_meta_unprotected(chunk_in_page),
	    2048 + (flash_meta_unprotected(chunk_in_page) - md_buffer), 2);
}

/*!
 @brief Write both metadata sections
 @param chunk_in_page The chunk index within the current page
 */
static inline void flash_write_metadata(uint8_t chunk){
	flash_write_meta_protected(chunk);
	flash_write_meta_unprotected(chunk);
}
