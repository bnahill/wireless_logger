#include "stm32f10x.h"
#include "lsm303.h"
#include "lpry.h"
#include "lps001.h"
#include "ff.h"
#include "rtc.h"
#include "logger.h"
#include "button.h"

#define LED_GPIO GPIOB
#define LED_PIN  BIT(9)
#define LED_CLK  RCC_APB2Periph_GPIOB

//! Flag for debugging to not log any data
#define DO_LOG 1

static volatile int tick = 0;

void led_init(void);
void led_set(void);
void led_clr(void);

int main(void){
	static volatile int i;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	rtc_init();
	lsm303_init();
	lpry_init();
	led_init();
	button_init();
	lps_init();
	
	while(1){
		while(!button_check_press());
	
		SysTick_Config(SystemCoreClock / 50);
#if DO_LOG
		if(logger_init("test") == NULL)
			while(1);
#endif
			
		led_set();
		
		i = 0;
		while(1){
			while(!tick);
			tick = 0;
			if(++i == 5){
				led_clr();
			} else if(i == 100){
				i = 0;
				led_set();
			}
			
			///////////////////////////////////////////
			// Start asynchronous sensor readings
			///////////////////////////////////////////
			
			// Start mag/acc readings
			lsm303_read();
			// Start pressure reading
			lps_read();
			// Start gyro reading
			lpry_read();
			
			///////////////////////////////////////////
			// Wait for results and update readings
			// DO NOT SET A BREAKPOINT DURING THIS TIME
			// I2C WILL CRASH AND NEED POWER CYCLE
			///////////////////////////////////////////
			
			// Wait for gyro read to be done
			while(!lpry_xfer_complete());
			// Update readings
			lpry_update();
			
			// Wait for LPS001 to get data
			while(!lps_xfer_complete());
			// Update its readings
			lps_update();
			
			// Wait for LSM303 to get data
			while(!lsm303_xfer_complete());
			// Update its readings
			lsm303_update();
			
			///////////////////////////////////////////
			// Update data logs
			///////////////////////////////////////////

#if DO_LOG		
			// Log the most recent readings
			logger_update();
#endif			
			if(button_check_press())
				break;
		}
		
		led_clr();
		
		// Disable SysTick
		SysTick_Config(SystemCoreClock);
#if DO_LOG	
		logger_sync();
		logger_close();
#endif
	}
}

void led_init(void){
	GPIO_InitTypeDef gpio_init_s;
	
	// Initialize GPIO
	RCC_APB2PeriphClockCmd(LED_CLK, ENABLE);
	
	led_clr();
	
	GPIO_StructInit(&gpio_init_s);
	gpio_init_s.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init_s.GPIO_Pin = LED_PIN;
	GPIO_Init(LED_GPIO, &gpio_init_s);
}

void led_set(void){
	LED_GPIO->BSRR = LED_PIN;
}

void led_clr(void){
	LED_GPIO->BSRR = LED_PIN << 16;
}

void SysTick_Handler(void){
	tick = 1;
}
