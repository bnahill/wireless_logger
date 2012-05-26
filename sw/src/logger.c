#include "logger.h"
#include "lsm303.h"
#include "lpry.h"
#include "lps001.h"
#include "ff.h"

#define LOG_BUFFER_LEN 8192

typedef struct {
	float gyro_pitch;
	float gyro_roll;
	float gyro_yaw;
	float acc_x;
	float acc_y;
	float acc_z;
	float mag_x;
	float mag_y;
	float mag_z;
	float pressure;
} log_frame_t;

// sizeof can't be used in preprocessor conditionals...
#define LOG_FRAME_SIZE (4 * 10)

#define LOG_BUFFER_FRAMES ((LOG_BUFFER_LEN - 4) / LOG_FRAME_SIZE)
#define LOG_BUFFER_PADDING ((LOG_BUFFER_LEN - 4) - (LOG_BUFFER_FRAMES * LOG_FRAME_SIZE))

static struct {
	uint32_t header;
	log_frame_t frames[LOG_BUFFER_FRAMES];
#if LOG_BUFFER_PADDING > 0
	uint32_t padding[LOG_BUFFER_PADDING / 4];
#endif
} write_buffer;

static log_frame_t *write_ptr;

static char filename[32];
static FATFS fs;
static FIL file;
static FRESULT res;

const char *logger_init(const char *prefix){
	char *ptr, *suffix;
	int i, j, tmp;
	FILINFO fi;
	if(f_mount(0, &fs) != FR_OK){
		return NULL;
	}
	// Copy the string and make a pointer to the beginning of the suffix
	for(suffix = filename; *prefix; suffix++, prefix++){
		*suffix = *prefix;
	}
	
	// Test suffixes in order starting from '1'
	for(i = 1; i < 10000; i++){
		ptr = suffix;
		j = i;
		if(j >= 1000){
			tmp = j / 1000;
			*(ptr++) = tmp + '0';
			j -= tmp * 1000;
		}
		if(j >= 100){
			tmp = j / 100;
			*(ptr++) = tmp + '0';
			j -= tmp * 100;
		}
		if(j >= 10){
			tmp = j / 10;
			*(ptr++) = tmp + '0';
			j -= tmp * 10;
		}

		*(ptr++) = j + '0';
		
		*ptr = 0;
		if(f_stat(filename, &fi) == FR_NO_FILE){
			break;
		}
	} if(i == 10000){
		f_mount(0, NULL);
		return NULL;
	}
	
	
	res = f_open(&file, filename, FA_READ | FA_WRITE | FA_CREATE_ALWAYS);
	
	if(res != FR_OK){
		return NULL;
	}
	
	write_ptr = write_buffer.frames;
	
	return filename;
}

void logger_update(void){
	write_ptr->gyro_pitch = gyro.reading.pitch;
	write_ptr->gyro_roll = gyro.reading.roll;
	write_ptr->gyro_yaw = gyro.reading.yaw;
	write_ptr->acc_x = magacc.acc.x;
	write_ptr->acc_y = magacc.acc.y;
	write_ptr->acc_z = magacc.acc.z;
	write_ptr->mag_x = magacc.mag.x;
	write_ptr->mag_y = magacc.mag.y;
	write_ptr->mag_z = magacc.mag.z;
	write_ptr->pressure = pressure.pressure;
	if(write_ptr - write_buffer.frames == LOG_BUFFER_FRAMES){
		// The page is full, write it
		logger_sync();
	} else {
		write_ptr += 1;
	}
}

void logger_sync(void){
	UINT write_len;
	// Write header section
	write_buffer.header = (write_ptr - 1 - write_buffer.frames) & 0x7FFFFFFF;
	
	// Check if data is empty
	if(write_buffer.header == 0)
		return;
		
	if(f_write(&file, (uint8_t *)&write_buffer, LOG_BUFFER_LEN, &write_len) != FR_OK)
		while(1);

	// Reset write pointer
	write_ptr = write_buffer.frames;
	// This doesn't really do anything but perhaps later it will
	f_sync(&file);
}

void logger_close(void){
	f_close(&file);
	f_mount(0, NULL);
}
