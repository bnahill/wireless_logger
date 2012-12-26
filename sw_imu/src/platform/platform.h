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

#include <hal.h>
#include <imu/i2c.h>
//#include <imu/spi.h>

#include <imu/mma8452q.h>
#include <imu/mag3110.h>



#ifdef __IMU_PLATFORM_CPP_ // Instantiate just once
I2C i2c1(I2CD1, OPMODE_I2C, FAST_DUTY_CYCLE_2, 100000);
#endif

extern I2C i2c1;

#ifdef __IMU_PLATFORM_CPP_ // Instantiate just once
MMA8452Q acc1(i2c1, 0x1C);
MAG3110 mag1(i2c1, 0x0E);
#endif

extern MMA8452Q acc1;
extern MAG3110 mag1;



///////////////////////////////////////////
// SPI Platform
///////////////////////////////////////////


#ifdef __SPI4_CPP_

///////////////////////////////////////////
// Declare all 4-wire SPI peripherals here
///////////////////////////////////////////

const SPI_4WireConfig SPI1_Config = {
	GPIO_AF_SPI1,
	{GPIOB, BIT(4), GPIO_PinSource4, RCC_Periph_GPIOB},
	{GPIOB, BIT(5), GPIO_PinSource5, RCC_Periph_GPIOB},
	{GPIOA, BIT(5), GPIO_PinSource5, RCC_Periph_GPIOB},
	1,
	DMA2_Stream0,
	DMA2_Stream3,
	DMA2,
	DMA_Channel_3,
	DMA_IT_TCIF0,
	DMA_FLAG_TCIF0,
	DMA_FLAG_TCIF3,
	DMA2_Stream0_IRQn,
	RCC_Periph_DMA2,
	SPI1_IRQn,
	RCC_Periph_SPI1
};


SPI_4Wire spi1(SPI1_Config, SPI1);

extern "C" {void DMA2_Stream0_IRQHandler();}
void DMA2_Stream0_IRQHandler(){
	spi1.isr_dma_rx();
}


#endif

#ifdef __SPI3_CPP_

///////////////////////////////////////////
// Declare all 3-wire SPI peripherals here
///////////////////////////////////////////

#endif

///////////////////////////////////////////
// End SPI platform
///////////////////////////////////////////







#endif // __IMU_PLATFORM_H_
