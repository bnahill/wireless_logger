/*!
 @file ssd1306.cpp
 @brief Templated body for \ref SSD1306 driver
 
 @note This file is not to be directly compiled; it must be included by
 whoever instantiates it.
 
 @author Ben Nahill <bnahill@gmail.com>
 */

#include "imu/ssd1306.h"
#include "imu/framebuffer.cpp"

template <uint32_t pages, uint32_t columns>
SSD1306<pages, columns>::SSD1306(SPI &spi, SPI::slave_config_t spi_config,
	                             gpio_pin_t nDC, gpio_pin_t nRES) :
	spi(spi), spi_config(spi_config), nDC(nDC), nRES(nRES)
{
	chMtxInit(&suspend_lock);
}

template <uint32_t pages, uint32_t columns>
void SSD1306< pages, columns >::init(){
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
	
	write_cmd(CMD_DISPLAYON);
	
	dump_fb();
}

template <uint32_t pages, uint32_t columns>
void SSD1306< pages, columns >::update(){
	fb.lock();
	lock();
	if(fb.get_limits().y_min <= fb.get_limits().y_max){
		write_cmd(CMD_SETPAGEADDRESS,
		          fb.get_limits().y_min,
		          fb.get_limits().y_max);
		transmit_data_sync(fb.get_fb(fb.get_limits().y_min),
		                   (1 + fb.get_limits().y_max -
		                      fb.get_limits().y_min)*columns);
		fb.sync(false);
	}
	unlock();
	fb.unlock();
}

template <uint32_t pages, uint32_t columns>
void SSD1306< pages, columns >::sleep(){
	lock();
	//nRES.clear();
	write_cmd(CMD_DISPLAYOFF);
	write_cmd(CMD_CHARGEPUMP, 0x10);
	//write_cmd(CMD_DISPLAYON);
}

template <uint32_t pages, uint32_t columns>
void SSD1306< pages, columns >::resume(){
	write_cmd(CMD_CHARGEPUMP, 0x14);
	write_cmd(CMD_DISPLAYON);
	unlock();
}

template <uint32_t pages, uint32_t columns>
void SSD1306< pages, columns >::dump_fb(){
	lock();
	write_cmd(0x22, 0, 3);
	transmit_data_sync(fb.get_fb(), pages*columns);
	unlock();
}

template <uint32_t pages, uint32_t columns>
void SSD1306< pages, columns >::write_cmd(uint8_t command){
	transmit_cmd_sync(&command, 1);
}

template <uint32_t pages, uint32_t columns>
void SSD1306< pages, columns >::write_cmd(uint8_t cmd1, uint8_t cmd2){
	uint8_t cmd[2] = {cmd1,cmd2};
	transmit_cmd_sync(cmd, 2);
}

template <uint32_t pages, uint32_t columns>
void SSD1306< pages, columns >::write_cmd(uint8_t cmd1, uint8_t cmd2,
	                                      uint8_t cmd3){
	uint8_t cmd[3] = {cmd1,cmd2,cmd3};
	transmit_cmd_sync(cmd, 3);
}

template <uint32_t pages, uint32_t columns>
void SSD1306< pages, columns >::transmit_cmd_sync(uint8_t const * buf,
	                                              uint8_t n){
	SPI::xfer_t xfer(&spi_config, n, buf);
	Semaphore done_sem;
	chSemInit(&done_sem, 0);
	
	xfer.starting_callback = (SPI::callback)callback_set_command;
	xfer.starting_param = (void*)&nDC;
	
	xfer.tc_sem(&done_sem);
	
	spi.transfer(xfer);
	
	chSemWait(&done_sem);
}

template <uint32_t pages, uint32_t columns>
void SSD1306< pages, columns >::transmit_data_sync(uint8_t const * buf,
	                                               uint32_t n){
	SPI::xfer_t xfer(&spi_config, n, buf);
	Semaphore done_sem;
	chSemInit(&done_sem, 0);
	
	xfer.starting_callback = (SPI::callback)callback_set_data;
	xfer.starting_param = (void *)&nDC;
	
	xfer.tc_sem(&done_sem);
	
	spi.transfer(xfer);
	
	chSemWait(&done_sem);
}

template <uint32_t pages, uint32_t columns>
void SSD1306< pages, columns >::callback_set_command(gpio_pin_t *gpio){
	gpio->clear();
}

template <uint32_t pages, uint32_t columns>
void SSD1306< pages, columns >::callback_set_data(gpio_pin_t *gpio){
	gpio->set();
}
