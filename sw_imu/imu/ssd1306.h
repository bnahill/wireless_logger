#ifndef __IMU_SSD1306_H_
#define __IMU_SSD1306_H_

#include "imu/imu.h"
#include "imu/spi.h"
#include "imu/framebuffer.h"

typedef enum {
	SSD1306_32_128,
	SSD1306_64_128,
} ssd1306_resolution;

template <uint32_t pages, uint32_t columns>
class SSD1306 {
public:
	SSD1306<pages, columns>(SPI &spi, SPI::slave_config_t spi_config, gpio_pin_t nDC, gpio_pin_t nRES) :
		spi(spi), spi_config(spi_config), nDC(nDC), nRES(nRES)
	{}
	
	void dump_fb(){
		write_cmd(0x22, 0, 3);
		transmit_data_sync(fb.get_fb(), pages*columns);
	}
	
	void update(){
		fb.lock();
		
		if(fb.get_limits().y_min <= fb.get_limits().y_max){
			write_cmd(CMD_SETPAGEADDRESS, fb.get_limits().y_min, fb.get_limits().y_max);
			transmit_data_sync(fb.get_fb(fb.get_limits().y_min), (1 + fb.get_limits().y_max - fb.get_limits().y_min)*columns);
			fb.sync(false);
		}
		
		fb.unlock();
	}
	
	void print_line(uint8_t line, char *text){
		
	}
	
	void init(){
		spi.init();
		
		nRES.clear();
		chThdSleep(MS2ST(10));
		nRES.set();
		chThdSleep(MS2ST(10));
		
		
		write_cmd(CMD_DISPLAYOFF);
		write_cmd(CMD_SETDISPLAYCLOCKDIV, 0x80);
		write_cmd(CMD_SETMULTIPLEX, val_multiplex);
		write_cmd(CMD_SETDISPLAYOFFSET, 0);
		write_cmd(CMD_SETSTARTLINE | 0x00);
		write_cmd(CMD_CHARGEPUMP, 0x14);
		write_cmd(CMD_MEMORYMODE, 0); // Horizontal addressing
		//write_cmd(CMD_SEGREMAP | 0x01);
		//write_cmd(CMD_COMSCANDEC);
		write_cmd(CMD_SEGREMAP);
		write_cmd(CMD_COMSCANINC);
		write_cmd(CMD_SETCOMPINS, val_compins);
		write_cmd(CMD_SETCONTRAST, val_contrast);
		write_cmd(CMD_SETPRECHARGE, val_precharge);
		write_cmd(CMD_SETVCOMDETECT, 0x40);
		write_cmd(CMD_DISPLAYALLON_RESUME);
		write_cmd(CMD_NORMALDISPLAY);
		
		dump_fb();
		
		write_cmd(CMD_DISPLAYON);
	}
	
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
	
	
	void write_cmd(uint8_t command){
		transmit_cmd_sync(&command, 1);
	}
	
	void write_cmd(uint8_t cmd1, uint8_t cmd2){
		uint8_t cmd[2] = {cmd1,cmd2};
		transmit_cmd_sync(cmd, 2);
	}

	void write_cmd(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3){
		uint8_t cmd[3] = {cmd1,cmd2,cmd3};
		transmit_cmd_sync(cmd, 3);
	}
	
	void transmit_cmd_sync(uint8_t const * buf, uint8_t n){
		SPI::xfer_t xfer(&spi_config, n, buf);
		Semaphore done_sem;
		chSemInit(&done_sem, 0);
		
		xfer.starting_callback = (SPI::callback)callback_set_command;
		xfer.starting_param = (void*)&nDC;
		
		xfer.tc_sem(&done_sem);
		
		spi.transfer(xfer);
		
		chSemWait(&done_sem);
	}
	
	void transmit_data_sync(uint8_t const * buf, uint32_t n){
		SPI::xfer_t xfer(&spi_config, n, buf);
		Semaphore done_sem;
		chSemInit(&done_sem, 0);
		
		xfer.starting_callback = (SPI::callback)callback_set_data;
		xfer.starting_param = (void *)&nDC;
		
		xfer.tc_sem(&done_sem);
		
		spi.transfer(xfer);
		
		chSemWait(&done_sem);
	}
	
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
	
	static void callback_set_command(gpio_pin_t *gpio){
		gpio->clear();
	}
	
	static void callback_set_data(gpio_pin_t *gpio){
		gpio->set();
	}
};

typedef SSD1306<4,128> LY091WG15;

#endif // __IMU_SSD1306_H_
