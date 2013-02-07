/*!
 @file platform.cpp
 
 @brief Platform-specific instantiations
 
 @author Ben Nahill <bnahill@gmail.com>
 */
#define __PLATFORM_CPP_

#include "platform.h"
#include "platform_usbserial.h"


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

LY091WG15 Platform::oled(spi2,
                        {NULL, GPIOB, 11, SPI_CR1_BR_1 |
                         SPI_CR1_CPOL | SPI_CR1_CPHA},
                        {GPIOB, 10}, {GPIOB, 2});

usbserial1_t Platform::usbserial1({GPIOA, 9});


///////////////////////////////////////////
// CC1101 platform configuration
///////////////////////////////////////////

// Sync word qualifier mode = 30/32 sync word bits detected 
// CRC autoflush = false 
// Channel spacing = 49.987793 
// Data format = Normal mode 
// Data rate = 499.878 
// RX filter BW = 541.666667 
// PA ramping = false 
// Preamble count = 8 
// Whitening = false 
// Address config = No address check 
// Carrier frequency = 916.500000 
// Device address = 0 
// TX power = 0 
// Manchester enable = false 
// CRC enable = true 
// Deviation = 1.586914 
// Packet length mode = Variable packet length mode. Packet length configured by the first byte after sync word 
// Packet length = 255 
// Modulation format = ASK/OOK 
// Base frequency = 916.500000 
// Channel number = 0 
static CC1101::reg_config_t const reg_config[] = {
	{CC1101::REG_IOCFG0, 0x06},
	{CC1101::REG_PKTCTRL0, 0x05},
	{CC1101::REG_FSCTRL1, 0x0E},
	{CC1101::REG_FREQ2, 0x23},
	{CC1101::REG_FREQ1, 0x40},
	{CC1101::REG_FREQ0, 0x00},
	{CC1101::REG_MDMCFG4, 0x2E},
	{CC1101::REG_MDMCFG3, 0x3B},
	{CC1101::REG_MDMCFG2, 0x33},
	{CC1101::REG_MDMCFG1, 0x40},
	{CC1101::REG_DEVIATN, 0x00},
	{CC1101::REG_MCSM0, 0x18},
	{CC1101::REG_FOCCFG, 0x1D},
	{CC1101::REG_BSCFG, 0x1C},
	{CC1101::REG_AGCCTRL2, 0xC7},
	{CC1101::REG_AGCCTRL1, 0x00},
	{CC1101::REG_AGCCTRL0, 0xB0},
	{CC1101::REG_FREND1, 0xB6},
	{CC1101::REG_FREND0, 0x11},
	{CC1101::REG_FSCAL3, 0xEA},
	{CC1101::REG_FSCAL2, 0x2A},
	{CC1101::REG_FSCAL1, 0x00},
	{CC1101::REG_FSCAL0, 0x1F},
	{CC1101::REG_TEST0, 0x09},
};

CC1101 Platform::rf1(Platform::spi1,
                     {NULL, GPIOC, 9, SPI_CR1_BR_2 | SPI_CR1_BR_1 |
                      SPI_CR1_CPOL | SPI_CR1_CPHA}, reg_config, sizeof(reg_config)/sizeof(*reg_config));

//////////////////////////////////////////////////////////
// Platform initialization
//////////////////////////////////////////////////////////

void Platform::early_init(){
	reg1.high_power(true);
	rf1.early_init();
	
	extStart(&EXTD1, &extcfg);
}

extern "C" {
void NMIVector(void){while(1);}
void HardFaultVector(void){while(1);}
void MemManageVector(void){while(1);}
void BusFaultVector(void){while(1);}
void UsageFaultVector(void){while(1);}
}
