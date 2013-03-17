/*!
 @file platform.cpp
 
 @brief Platform-specific instantiations
 
 @author Ben Nahill <bnahill@gmail.com>
 */
#define __PLATFORM_CPP_

#include "platform.h"
#include "platform_usbserial.h"

#include "imu/ssd1306.cpp"


using namespace Platform;


I2C Platform::i2c1(I2CD1, OPMODE_I2C, FAST_DUTY_CYCLE_2, 100000);
MMA8452Q Platform::acc1(Platform::i2c1, 0x1C);
MAG3110 Platform::mag1(Platform::i2c1, 0x0E);

///////////////////////////////////////////
// SPI Platform
///////////////////////////////////////////

SPI Platform::spi1(SPID1);

SPI Platform::spi2(SPID2);

///////////////////////////////////////////
// L3GD20 Platform
///////////////////////////////////////////

L3GD20_SPI Platform::gyro1(Platform::spi1, {NULL, GPIOC, 4, SPI_CR1_BR_2 |
                                            SPI_CR1_BR_1 | SPI_CR1_CPOL |
                                            SPI_CR1_CPHA});


///////////////////////////////////////////
// Button platform configuration
///////////////////////////////////////////

button_t Platform::button[3] = {
	{GPIOC, 12, button_t::ACTIVE_LOW},
	{GPIOC, 10, button_t::ACTIVE_LOW},
	{GPIOC, 11, button_t::ACTIVE_LOW}
};

///////////////////////////////////////////
// Edge-trigger interrupt configuration
///////////////////////////////////////////

const EXTConfig Platform::extcfg = {
	{
		{EXT_CH_MODE_DISABLED, NULL}, // 0
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL}, // 5
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOC,
			Ext1Callback::callback}, // 10
		{EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOC,
			Ext1Callback::callback},
		{EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOC,
			Ext1Callback::callback},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL}, // 15
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL}, // 20
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL}
	}
};

///////////////////////////////////////////
// LED platform configuration
///////////////////////////////////////////

gpio_pin_t Platform::led1(GPIOC, 3);


LTC3559 Platform::reg1({GPIOC, 2}, {GPIOC, 1}, {GPIOA, 4}, {GPIOA, 1});

///////////////////////////////////////////
// OLED display platform configuration
///////////////////////////////////////////


template class SSD1306<4,128>;
template class FrameBuffer<4, 128>;


LY091WG15 Platform::oled(spi2,
                        {NULL, GPIOB, 11, SPI_CR1_BR_1 |
                         SPI_CR1_CPOL | SPI_CR1_CPHA},
                        {GPIOB, 10}, {GPIOB, 2});

usbserial1_t Platform::usbserial1({GPIOA, 9});


///////////////////////////////////////////
// CC1101 platform configuration
///////////////////////////////////////////

CC1101 Platform::rf1(Platform::spi1,
                     {NULL, GPIOC, 9, SPI_CR1_BR_2 | SPI_CR1_BR_1 |
                      SPI_CR1_CPOL | SPI_CR1_CPHA});

GuardianRF Platform::guardian1(rf1);

//////////////////////////////////////////////////////////
// Flash platform configuration
//////////////////////////////////////////////////////////

MT29FxG01 Platform::flash(spi2, MT29FxG01::SIZE_1G,
                         {GPIOB, 12}, {GPIOA, 8}, {GPIOC, 6}, 
                         (uint32_t)(SPI_CR1_BR_0 | SPI_CR1_CPOL | SPI_CR1_CPHA));

//////////////////////////////////////////////////////////
// Platform initialization
//////////////////////////////////////////////////////////

void Platform::early_init(){
	reg1.high_power(true);
	reg1.buck_mode(LTC3559::MODE_BURST);
	rf1.early_init();
	
	extStart(&EXTD1, &extcfg);
}


// Lockable * const Platform::buses[] = {
// 	&i2c1,
// 	&spi1,
// 	&spi2,
// 	nullptr
// };

extern "C" {
void NMIVector(void){while(1);}
void HardFaultVector(void){
	asm volatile ("MRS R0,PSP");
	asm volatile ("LDR R1,[R0,#24]");

	while(1);
}
void MemManageVector(void){while(1);}
void BusFaultVector(void){while(1);}
void UsageFaultVector(void){while(1);}
}
