/*!
 @file platform.cpp
 
 @brief Platform-specific instantiations
 
 @author Ben Nahill <bnahill@gmail.com>
 */
#define __PLATFORM_CPP_

#include "platform.h"
#include "platform_usbserial.h"
#include "flogfs.h"

#include "imu/ssd1306.cpp"
#include "imu/datasource.cpp"
#include "imu/clock_mgr.h"


using namespace Platform;

template <GPIO_TypeDef * gpio_temp,
          uint32_t mask,
          uint32_t direction,
          uint32_t pull,
          uint32_t af>
class pin_t {
	static constexpr GPIO_TypeDef * gpio = gpio_temp;
};


I2C Platform::i2c1(I2CD1, OPMODE_I2C, FAST_DUTY_CYCLE_2, 400000, {GPIOB, 7}, {GPIOB, 6});
MMA8452Q Platform::acc1(Platform::i2c1, 0x1C);
MAG3110 Platform::mag1(Platform::i2c1, 0x0E);


template class DataSource<Euclidean3_f32>;
template class DataListener<Euclidean3_f32>;
DataSource<Euclidean3_f32> Platform::acc_source;
DataSource<Euclidean3_f32> Platform::mag_source;
DataSource<Euclidean3_f32> Platform::gyro_source;

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
                        {NULL, GPIOB, 11, SPI_CR1_BR_0 |
                         SPI_CR1_CPOL | SPI_CR1_CPHA},
                        {GPIOB, 10}, {GPIOB, 2});

usbserial1_t Platform::usbserial1({GPIOA, 9});


///////////////////////////////////////////
// CC1101 platform configuration
///////////////////////////////////////////

// CC1101 Platform::rf1(Platform::spi1,
//                      {NULL, GPIOC, 9, SPI_CR1_BR_2 | SPI_CR1_BR_1});

CC1101 Platform::rf1(Platform::spi1,
                     {NULL, GPIOA, 15, SPI_CR1_BR_2 | SPI_CR1_BR_1});

GuardianRF Platform::guardian1(rf1);

//////////////////////////////////////////////////////////
// Flash platform configuration
//////////////////////////////////////////////////////////

MT29FxG01 Platform::flash(spi2, MT29FxG01::SIZE_1G,
                         {GPIOB, 12}, {GPIOA, 8}, {GPIOC, 6}, 
                         (SPI_CR1_CPOL | SPI_CR1_CPHA));



//////////////////////////////////////////////////////////
// Platform initialization
//////////////////////////////////////////////////////////

EventLog Platform::evt_log;

void Platform::early_init(){
	clk_mgr_init();
	
	reg1.high_power(true);
	reg1.buck_mode(LTC3559::MODE_BURST);
	rf1.early_init();
	evt_log.init();
	if(!flogfs_init()){
		evt_log.add("FLogFS init\n failed!", EventItem::SEVERITY_ERROR);
	} else {
		if(!flogfs_mount()){
			evt_log.add("FLogFS mount\n failed!", EventItem::SEVERITY_ERROR);
		}
	}
	//flogfs_mount();

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
