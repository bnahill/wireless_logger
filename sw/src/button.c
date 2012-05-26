#include "button.h"

#define BUTTON_GPIO         GPIOC
#define BUTTON_PORT_SRC     GPIO_PortSourceGPIOC
#define BUTTON_PIN          BIT(13)
#define BUTTON_PIN_SRC      GPIO_PinSource13
#define BUTTON_EXTI         EXTI_Line13
#define BUTTON_EXTI_IRQ     EXTI15_10_IRQn
#define BUTTON_EXTI_ISR     EXTI15_10_IRQHandler

#define BUTTON_GPIO_CLK_CMD RCC_APB2PeriphClockCmd
#define BUTTON_GPIO_CLK     RCC_APB2Periph_GPIOC

static enum {BUTTON_ON, BUTTON_OFF} button_state;
static uint16_t button_tmr_prescaler;
static int has_been_pressed = 0;

static void button_set_off(void){
	button_state = BUTTON_OFF;
	// Detect only falling
	EXTI->RTSR &= ~BUTTON_PIN;
	EXTI->FTSR |= BUTTON_PIN;
}

static void button_set_on(void){
	button_state = BUTTON_ON;
	// Detect rising to release button
	EXTI->RTSR |= BUTTON_PIN;
	EXTI->FTSR &= ~BUTTON_PIN;
}

static void button_start_timer(void){
	TIM_PrescalerConfig(TIM2, button_tmr_prescaler, TIM_PSCReloadMode_Immediate);
	TIM2->CNT = 0;
	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void button_init(void){
	GPIO_InitTypeDef gpio_init_s;
	EXTI_InitTypeDef exti_init_s;
	NVIC_InitTypeDef nvic_init_s;
	TIM_TimeBaseInitTypeDef  timebase_s;
	RCC_ClocksTypeDef rcc_clocks;

	RCC_GetClocksFreq(&rcc_clocks);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// Initialize GPIO
	BUTTON_GPIO_CLK_CMD(BUTTON_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_StructInit(&gpio_init_s);
	gpio_init_s.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_init_s.GPIO_Pin = BUTTON_PIN;
	GPIO_Init(BUTTON_GPIO, &gpio_init_s);
	
	GPIO_EXTILineConfig(BUTTON_PORT_SRC, BUTTON_PIN_SRC);
	
	// Configure EXTI
	exti_init_s.EXTI_Line = BUTTON_EXTI;
	exti_init_s.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_init_s.EXTI_Trigger = EXTI_Trigger_Falling;
	exti_init_s.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti_init_s);
	
	timebase_s.TIM_Period = 65535;
	timebase_s.TIM_Prescaler = 0;
	timebase_s.TIM_ClockDivision = 0;
	timebase_s.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &timebase_s);
	
	button_tmr_prescaler = 65536 * (rcc_clocks.PCLK1_Frequency / 50) - 1;
	
	// Configure NVIC
	nvic_init_s.NVIC_IRQChannel = TIM2_IRQn;
	nvic_init_s.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_init_s.NVIC_IRQChannelSubPriority = 1;
	nvic_init_s.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_init_s);
	
	nvic_init_s.NVIC_IRQChannel = BUTTON_EXTI_IRQ;
	nvic_init_s.NVIC_IRQChannelPreemptionPriority = 0x0F;
	nvic_init_s.NVIC_IRQChannelSubPriority = 0x0F;
	nvic_init_s.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_init_s);
	
	if(BUTTON_GPIO->IDR & BUTTON_PIN){
		button_set_off();
	} else {
		button_set_on();
	}
}

int button_check_press(void){
	if(has_been_pressed){
		has_been_pressed = 0;
		return 1;
	}
	return 0;
}

void BUTTON_EXTI_ISR(void){
  if(EXTI_GetITStatus(BUTTON_EXTI) != RESET)
  {
    /* Clear the  EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(BUTTON_EXTI);
	switch(button_state){
	case BUTTON_ON:
		// Check if it's already gone away
		if((BUTTON_GPIO->IDR & BUTTON_PIN) == 0)
			return;

		EXTI->RTSR &= ~BUTTON_PIN;
		EXTI->FTSR &= ~BUTTON_PIN;
		button_start_timer();
		break;
	case BUTTON_OFF:
		// Check if it's already gone away
		if((BUTTON_GPIO->IDR & BUTTON_PIN) != 0)
			return;
			
		EXTI->RTSR &= ~BUTTON_PIN;
		EXTI->FTSR &= ~BUTTON_PIN;
		button_start_timer();
		break;
	}
  }
}



void TIM2_IRQHandler(void){
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
		TIM_Cmd(TIM2, DISABLE);
		switch(button_state){
		case BUTTON_ON:
			if((BUTTON_GPIO->IDR & BUTTON_PIN) != 0){
				button_set_off();
			}
			break;
		case BUTTON_OFF:
			if((BUTTON_GPIO->IDR & BUTTON_PIN) == 0){
				button_set_on();
				has_been_pressed = 1;
			}
			break;
		}
	}
}
