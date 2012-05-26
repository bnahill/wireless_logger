#include "fs_hal.h"
#include "sdio.h"
#include "rtc.h"

static int is_init = 0;

typedef struct _sd_xfer {
	enum {SD_OP_READ, SD_OP_WRITE} op;
	uint32_t address;
	uint8_t *buffer;
	uint16_t num_blocks;
	struct _sd_xfer *next;
} sd_xfer_t;

typedef struct {
	
	sd_xfer_t *xfer;
} sd_state_t;

static struct {
	uint32_t block_size;
	int is_init;
} sd_disk;

DSTATUS disk_initialize (BYTE disk){
	static SD_CardInfo ci;
	if(SD_Init() == SD_OK){
		is_init = 1;
		SD_GetCardInfo(&ci);
		sd_disk.block_size = ci.CardBlockSize;
		return 0;
	}
	return disk_status(disk);
}

DSTATUS disk_status (BYTE disk){
	if(!is_init)
		return STA_NOINIT;
	return 0;
}

DRESULT disk_read (BYTE disk, BYTE* buf, DWORD sector, BYTE count){
	// Check alignment
	if((int)buf & 3 != 0)
		return RES_ERROR;

	if(count > 1){
		if(SD_ReadMultiBlocks((uint32_t *)buf, sector * 512, 512, count) != SD_OK)
			return RES_ERROR;
	} else {
		if(SD_ReadBlock((uint32_t *)buf, sector * 512,  512) != SD_OK)
			return RES_ERROR;
	}
	
	if(SD_Wait() != SD_OK)
		return RES_ERROR;

	return RES_OK;
}

DRESULT disk_write (BYTE disk, const BYTE* buf, DWORD sector, BYTE count){
	// Check alignment
	if((int)buf & 3 != 0)
		return RES_ERROR;

	if(count > 1){
		if(SD_WriteMultiBlocks((const uint32_t *)buf, sector * 512,  512, count) != SD_OK)
			return RES_ERROR;
	} else {
		if(SD_WriteBlock((const uint32_t *)buf, sector * 512,  512) != SD_OK)
			return RES_ERROR;
	}
	
	if(SD_Wait() != SD_OK)
		return RES_ERROR;

	return RES_OK;
}

DRESULT disk_ioctl (BYTE disk, BYTE cmd, void* buf){
	DRESULT ret = RES_OK;
	
	switch(cmd){
	case CTRL_SYNC:
		SD_Wait();
		break;
	case GET_SECTOR_SIZE:
		*(DWORD *)buf = 512;//sd_disk.block_size;
		break;
	case GET_SECTOR_COUNT:
		// Unimplemented
		break;
	case GET_BLOCK_SIZE:
		*(DWORD *)buf = 1;
		break;
	case CTRL_ERASE_SECTOR:
		break;
	default:
		while(1);
	}
	
	return ret;
}

DWORD get_fattime (void){
	datetime_t dt;
	DWORD res;
	
	rtc_get_datetime(&dt);
	
	// Year since 1980
	res = (dt.year - 1980) << 25;
	// Month
	res |= dt.month << 21;
	// Day
	res |= dt.day << 16;
	// Hour
	res |= dt.hour << 11;
	// Minute
	res |= dt.minute << 5;
	// Second / 2
	res |= dt.second >> 1;
	
	return res;
}
