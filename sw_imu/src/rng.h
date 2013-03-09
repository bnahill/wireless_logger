#ifndef __RNG_H_
#define __RNG_H_

#include "ch.h"
#include "hal.h"

class Random {
public:
	static void init(){
		RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
		RNG->CR = 0;
		RNG->CR = RNG_CR_RNGEN;
		while((RNG->SR & RNG_SR_DRDY) == 0);
	}
	
	static void close(){
		RNG->CR = 0;
		RCC->AHB2ENR &= ~RCC_AHB2ENR_RNGEN;
	}
	
	static uint32_t get_value(){
		while((RNG->SR & RNG_SR_DRDY) == 0);
		return RNG->DR;
	}
};

#endif
