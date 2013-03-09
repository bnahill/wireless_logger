/*!
 @file 
 
 @brief 
 
 @author Ben Nahill <bnahill@gmail.com>
 */

#ifndef __IMU_SSD1306_H_
#define __IMU_SSD1306_H_

#include "imu/imu.h"
#include "imu/spi.h"
#include "imu/framebuffer.h"

//! @addtogroup IMU
//! @{
//! @addtogroup Display
//! @{

template <uint32_t pages, uint32_t columns>
class SSD1306 {
public:
	SSD1306<pages, columns>(SPI &spi, SPI::slave_config_t spi_config, gpio_pin_t nDC, gpio_pin_t nRES);
	
	void dump_fb();
	
	void update();
	
	void sleep();
	
	void resume();
	
	void init();
	
	FrameBuffer<pages,columns> fb;
protected:
	//! Commands
	typedef enum {
		CMD_SETCONTRAST         = 0x81,
		CMD_DISPLAYALLON_RESUME = 0xA4,
		CMD_DISPLAYALLON        = 0xA5,
		CMD_NORMALDISPLAY       = 0xA6,
		CMD_INVERTDISPLAY       = 0xA7,
		CMD_DISPLAYOFF          = 0xAE,
		CMD_DISPLAYON           = 0xAF,

		CMD_SETDISPLAYOFFSET    = 0xD3,
		CMD_SETCOMPINS          = 0xDA,

		CMD_SETVCOMDETECT       = 0xDB,

		CMD_SETDISPLAYCLOCKDIV  = 0xD5,
		CMD_SETPRECHARGE        = 0xD9,

		CMD_SETMULTIPLEX        = 0xA8,

		CMD_SETLOWCOLUMN        = 0x00,
		CMD_SETHIGHCOLUMN       = 0x10,

		CMD_SETSTARTLINE        = 0x40,

		CMD_MEMORYMODE          = 0x20,
		CMD_SETCOLADDRESS       = 0x21,
		CMD_SETPAGEADDRESS      = 0x22,

		CMD_COMSCANINC          = 0xC0,
		CMD_COMSCANDEC          = 0xC8,

		CMD_SEGREMAP            = 0xA0,

		CMD_CHARGEPUMP          = 0x8D,

		CMD_EXTERNALVCC         = 0x1,
		CMD_SWITCHCAPVCC        = 0x2
	} cmd_t;
	
	
	void write_cmd(uint8_t command);
	void write_cmd(uint8_t cmd1, uint8_t cmd2);
	void write_cmd(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3);
	
	void transmit_cmd_sync(uint8_t const * buf, uint8_t n);
	
	void transmit_data_sync(uint8_t const * buf, uint32_t n);
	
	void set_start_page(uint8_t page){
		
	}
	
	
	void write_data(uint8_t const * data);
	
	/*!
	 @name Configuration for the panel
	 All of these should be overridden in specializations
	 @{
	 */
	static const uint8_t val_compins;
	static const uint8_t val_multiplex;
	static const uint8_t val_precharge;
	static const uint8_t val_contrast;
	/*!
	 @}
	 */
	
	gpio_pin_t const nDC, nRES;
	
	SPI &spi;
	SPI::slave_config_t spi_config;
	
	Mutex suspend_lock;
	
	void lock(){chMtxLock(&suspend_lock);}
	void unlock(){chMtxUnlock();}
	
	static void callback_set_command(gpio_pin_t *gpio);
	
	static void callback_set_data(gpio_pin_t *gpio);
};

typedef SSD1306<4,128> LY091WG15;

//! @} @}

#endif // __IMU_SSD1306_H_
