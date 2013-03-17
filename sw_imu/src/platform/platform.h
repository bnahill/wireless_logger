/*!
 @file platform.h
 
 @brief Platform-specific details for most of the logging hardware
 
 This file should exist for each different supported platform. It should
 indicate what buses are used for each device and how they should be
 configured.
 
 Most of the wiring details are left to board.h where the I/O modes for port
 configurations are defined. SPI slaves are an exception.
 
 @author Ben Nahill <bnahill@gmail.com>
 */

#ifndef __IMU_PLATFORM_H_
#define __IMU_PLATFORM_H_

#include <ch.h>
#include <hal.h>

#include "imu/imu.h"

#include <imu/i2c.h>
#include <imu/spi.h>

#include <imu/mma8452q.h>
#include <imu/mag3110.h>
#include <imu/ext.h>
#include <imu/button.h>
#include <imu/l3gd20.h>
#include <imu/ltc3559.h>
#include <imu/ssd1306.h>
#include <imu/usbserial.h>
#include <imu/rtc.h>
#include <imu/cc1101.h>
#include <imu/guardian.h>
#include <imu/mt29fxg01.h>
#include "rng.h"

/*!
 @brief Platform-dependent component definitions
 
 This namespace contains all platform-dependent devices. This includes all
 external peripherals and their on-chip interfaces. All of these items shall
 be instantiantiated in platform.cpp.
 */
namespace Platform {
	extern I2C i2c1;
	extern MMA8452Q acc1;
	extern MAG3110 mag1;
	
	extern SPI spi1;
	extern SPI spi2;
	extern L3GD20_SPI gyro1;
	
	typedef ExtCallback<EXTD1> Ext1Callback;
	typedef Button<EXTD1> button_t;
	extern button_t button[3];
	
	extern gpio_pin_t led1;
	
	extern const EXTConfig extcfg;
	
	extern LTC3559 reg1;
	
	extern LY091WG15 oled;
	
	typedef USBSerial<USBD1> usbserial1_t;
	extern usbserial1_t usbserial1;
	
	typedef RealTimeClock<RTCD1> rtc1;
	
	extern CC1101 rf1;
	extern GuardianRF guardian1;
	
	extern MT29FxG01 flash;
	
	//extern Lockable * const buses[];
	
	void early_init();
};

//using namespace Platform;

#endif // __IMU_PLATFORM_H_
