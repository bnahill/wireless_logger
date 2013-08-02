#include "imu/clock_mgr.h"

#if USE_CLK_MGR

uint32_t hse_sem = 0;
uint32_t hsi_sem = 0;

#define DISABLE_REG TRUE

typedef enum {
	PLLSRC_HSI = 0,
	PLLSRC_HSE = 1 << 22
} pllsrc_t;

#define MK_PLL(pllsrc, pllm, plln, pllp, pllq) ((uint32_t) \
	(pllm | (plln << 6) | (((pllp >> 1) -1) << 16) | (pllsrc) | (pllq << 24)))



#define HSE8_PLL_VALUE  MK_PLL(PLLSRC_HSE, 4, 168, 2, 7)
#define HSE25_PLL_VALUE MK_PLL(PLLSRC_HSE, 25, 336, 2, 7)
#define HSI16_PLL_VALUE MK_PLL(PLLSRC_HSI, 8, 168, 2, 7)

void clk_mgr_init(){
	PWR->CR &= ~PWR_CR_PDDS;
#if DISABLE_REG
	PWR->CR |= PWR_CR_LPDS;
#endif
}

void clk_mgr_noreq_hse(){
	chSysLock();
	if(hse_sem){
		hse_sem -= 1;
		if(!hsi_sem && !hse_sem){
			SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
		}
		if(!hse_sem){
			// SHUT IT OFF!
			
			// Enable HSI
			RCC->CR |= RCC_CR_HSION;
			// Wait for it
			while ((RCC->CR & RCC_CR_HSIRDY) == 0);
			// Switch to HSI
			RCC->CFGR = (RCC->CFGR & ~3);
			// Disable PLL
			RCC->CR &= ~RCC_CR_PLLON;
			// Configure PLL
			RCC->PLLCFGR = HSI16_PLL_VALUE;
			
			// Re-enable PLL
			RCC->CR |= RCC_CR_PLLON;
			// Wait for it
			while((RCC->CR | RCC_CR_PLLRDY) == 0);
			
			// Switch to PLL for HCLK
			RCC->CFGR = (RCC->CFGR & ~3);
			
			// Disable HSE!
			RCC->CR &= ~RCC_CR_HSEON;
		}
	}
	chSysUnlock();
}

void clk_mgr_noreq_hsi(){
	chSysLock();
	if(hsi_sem){
		hsi_sem -= 1;
		if(!hsi_sem && !hse_sem){
			SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
		}
	}
	chSysUnlock();
}

void clk_mgr_req_hse(){
	chSysLock();
	hse_sem += 1;
	// Don't allow deep sleep
	SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;	
	if(hse_sem == 1){
		// Only task requiring HSE
		// Enable it
		RCC->CR |= RCC_CR_HSEON;
		while ((RCC->CR & RCC_CR_HSERDY) == 0);
		// Switch to HSE
		RCC->CFGR = (RCC->CFGR & ~3) | 1;
		// Disable PLL
		RCC->CR &= ~RCC_CR_PLLON;
		// Configure PLL
		RCC->PLLCFGR = HSE8_PLL_VALUE;
		// Re-enable PLL
		RCC->CR |= RCC_CR_PLLON;
		// Wait for PLL
		while((RCC->CR | RCC_CR_PLLRDY) == 0);
		// Switch to PLL for HCLK
		RCC->CFGR = (RCC->CFGR & ~3) | 2;
		
		// WHO NEEDS THE HSI!?
		RCC->CR &= ~RCC_CR_HSION;
	}
	chSysUnlock();
}

void clk_mgr_req_hsi(){
	chSysLock();
	hsi_sem += 1;
	// Don't allow deep sleep
	SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;	
	chSysUnlock();
}

void clk_mgr_wakeup(){
	chSysLockFromIsr();
#if DISABLE_REG
	// Wait for regulator
	while((PWR->CSR & PWR_CSR_VOSRDY) == 0);
#endif
	if((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){
		// FOR SOME REASON THE SOURCE IS PROBABLY HSI
		if((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_HSE){
			// Just to be sure...
			while(1);
		}
		
		// Disable PLL
		RCC->CR &= ~RCC_CR_PLLON;
		// Configure PLL
		RCC->PLLCFGR = HSI16_PLL_VALUE;
		
		// Re-enable PLL
		RCC->CR |= RCC_CR_PLLON;
		// Wait for it
		while((RCC->CR | RCC_CR_PLLRDY) == 0);
		
		// Switch to PLL for HCLK
		RCC->CFGR = (RCC->CFGR & ~3) | 2;
	}
	
	chSysUnlockFromIsr();
	
	// Enable HSI -- already on?
	
	// Wait for HSI -- probably fine...
	
	// Select HSI as PLL input -- probably already set...
	
	// Load PLL values -- again...
	
	// Enable PLL
	
	// Wait for PLL
}

void clk_mgr_gosleep(){
	// Select HSI for PLL and load values to avoid doing so on wakeup
	// Go to sleep
}

#endif // USE_CLK_MGR
