#ifndef __POWER_MANAGEMENT_H_
#define __POWER_MANAGEMENT_H_

#include "ch.h"
#include "hal.h"

class PowerManagement {
public:
	typedef enum {
		SPEED_24,
		SPEED_48,
		SPEED_64,
		SPEED_168
	} speed_t;
	
	static void set_speed(speed_t speed){
		chSysLock();
		if(speed == current_speed) {
			chSysUnlock();
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
};

#endif // __POWER_MANAGEMENT_H_
