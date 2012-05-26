#include "sdio_ll.h"


void SD_LowLevel_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// Enable the SDIO AHB Clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, ENABLE);
	
	// Enable the DMA2 Clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	
	// Enable the The two GPIO ports involved
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |
	                       RCC_APB2Periph_GPIOD, ENABLE);
	
	// GPIOC and GPIOD Periph clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	
	// Configure PC.08, PC.09, PC.10, PC.11, PC.12 pin: D0, D1, D2, D3, CLK pin
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// Configure PD.02 CMD line
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void SD_LowLevel_DeInit(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Disable SDIO Clock
	SDIO_ClockCmd(DISABLE);

	// Set Power State to OFF
	SDIO_SetPowerState(SDIO_PowerState_OFF);

	// DeInitializes the SDIO peripheral
	SDIO_DeInit();

	// Disable the SDIO APB2 Clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, DISABLE);

	// Configure PC.08, PC.09, PC.10, PC.11, PC.12 pins: D0, D1, D2, D3, CLK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Configure PD.02 CMD line
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void SD_LowLevel_DMA_RxConfig(uint32_t *dst, uint32_t cnt){
	DMA_InitTypeDef DMA_InitStructure;
	
	DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);
	
	// DMA2 Channel4 disable
	DMA_Cmd(DMA2_Channel4, DISABLE);
	
	// DMA2 Channel4 Config
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)SDIO_FIFO_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)dst;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = cnt / 4;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel4, &DMA_InitStructure);
	
	// DMA2 Channel4 enable
	DMA_Cmd(DMA2_Channel4, ENABLE);
}

void SD_LowLevel_DMA_TxConfig(const uint32_t *src, uint32_t cnt){
	DMA_InitTypeDef DMA_InitStructure;
	
	DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);
	
	// DMA2 Channel4 disable
	DMA_Cmd(DMA2_Channel4, DISABLE);
	
	// DMA2 Channel4 Config
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)SDIO_FIFO_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)src;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = cnt / 4;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel4, &DMA_InitStructure);
	
	// DMA2 Channel4 enable
	DMA_Cmd(DMA2_Channel4, ENABLE);
}


FlagStatus SD_DMAEndOfTransferStatus(void){
	return DMA_GetFlagStatus(DMA2_FLAG_TC4);
}

void SDIO_IRQHandler(void){
	SD_ProcessIRQSrc();
}
