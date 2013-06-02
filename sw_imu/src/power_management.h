#ifndef __POWER_MANAGEMENT_H_
#define __POWER_MANAGEMENT_H_

#include "ch.h"
#include "hal.h"
#include "platform/platform.h"

class PowerManagement {
public:
	typedef enum {
		SPEED_24,
		SPEED_48,
		SPEED_64,
		SPEED_168
	} speed_t;
	
	static void set_speed(speed_t speed){
		lock_buses();
		chSysLock();
		if(speed == current_speed) {
			chSysUnlock();
			unlock_buses();
			return;
		}
		
		switch(speed){
		case SPEED_24:
			set_pll(PLLSRC_HSE, 8*4, 336/2, 2, 7);
			break;
		case SPEED_48:
			set_pll(PLLSRC_HSE, 8*2, 336/2, 2, 7);
			break;
		case SPEED_64:
			set_pll(PLLSRC_HSE, 8, 336/2, 2, 7);
			break;
		case SPEED_168:
			set_pll(PLLSRC_HSE, 8, 336, 2, 7);
			break;
		}
		
		current_speed = speed;
		chSysUnlock();
		unlock_buses();
	}
	
	static void enable_rtc_tick(){
		// Enable the RTC wakeup EXTI line
		EXTI->RTSR |= (1 << 22);
		EXTI->FTSR &= ~(1 << 22);
		EXTI->IMR |= (1 << 22);
		
		RTC->WUTR = floorf((16384.0 / CH_FREQUENCY) + 0.5f) - 1;
		// Enable wakeup timer
		RTC->CR &= ~RTC_CR_WUTE;
		RTC->CR |= RTC_CR_WUTE | RTC_CR_WUTIE |
				RTC_CR_WUCKSEL_1 | RTC_CR_WUCKSEL_0; // 16384Hz
		nvicEnableVector(RTC_WKUP_IRQn,
						CORTEX_PRIORITY_MASK(CORTEX_PRIORITY_SYSTICK));
		RTC->ISR &= ~RTC_ISR_WUTF;
	}

	static void disable_systick(){
		// Disable the regular tick interrupt
		SysTick->CTRL = 0;
		nvicDisableVector(HANDLER_SYSTICK);	
	}

protected:
	static speed_t current_speed;
	
	typedef enum {
		PLLSRC_HSI = 0,
		PLLSRC_HSE = 1 << 22
	} pllsrc_t;
	
	static void set_pll(pllsrc_t pllsrc, uint32_t pllm, uint32_t plln, uint32_t pllp, uint32_t pllq){
		RCC->CFGR = (RCC->CFGR & ~3) | 1;
		RCC->CR &= ~RCC_CR_PLLON;
		RCC->PLLCFGR = pllm | (plln << 6) | (((pllp >> 1) -1) << 16) | (pllsrc) | (pllq << 24);
		RCC->CR |= RCC_CR_PLLON;
		while(!pll_ready());
		RCC->CFGR = (RCC->CFGR & ~3) | 2;
		
		SysTick->CTRL = 0;
		SysTick->LOAD = ((STM32_HSECLK * plln) / (pllm * pllp * CH_FREQUENCY)) - 1;
		SysTick->VAL = 0;
		SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
		SysTick_CTRL_ENABLE_Msk |
		SysTick_CTRL_TICKINT_Msk;
	}
	
	static bool pll_ready(){
		return RCC->CR | RCC_CR_PLLRDY;
	}
	
	static void lock_buses(){
// 		Lockable * const * iter;
// 		for(iter = Platform::buses; *iter; iter++){
// 			(*iter)->lock();
// 		}
	}
	
	static void unlock_buses(){
// 		Lockable * const * iter;
// 		for(iter = Platform::buses; *iter; iter++){
// 			(*iter)->unlock();
// 		}
	}
};

#endif // __POWER_MANAGEMENT_H_
