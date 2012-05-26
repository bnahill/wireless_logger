#include "lpry.h"

#define GYRO_ADC                  ADC1
#define GYRO_RCC_Periph_ADC       RCC_APB2Periph_ADC1
#define GYRO_ADC_Port             GPIOC
#define GYRO_ADC_DR_Address       ((u32)0x4001244C)
#define GYRO_RCC_Port             RCC_APB2Periph_GPIOC

#define GYRO_DMA                  DMA1
#define GYRO_RCC_Periph_DMA       RCC_AHBPeriph_DMA1
#define GYRO_DMA_Channel          DMA1_Channel1
#define GYRO_DMA_IRQn             DMA1_Channel1_IRQn
#define GYRO_DMA_ISR              DMA1_Channel1_IRQHandler
#define GYRO_DMA_FLAG             DMA1_FLAG_TC1

//! GPIO for ST and PD/SLEEP pin
#define GYRO_GPIO_Port2            GPIOB
#define GYRO_GPIO_RCC_Port2        RCC_APB2Periph_GPIOB

#define GYRO_LY_PinZ              BIT(4)
#define GYRO_LY_PinREF            BIT(5)
#define GYRO_LY_PinST             BIT(14)
#define GYRO_LY_PinPD             BIT(15)

#define GYRO_LPR_PinY             BIT(1)
#define GYRO_LPR_PinX             BIT(0)
#define GYRO_LPR_PinREF           BIT(2)
#define GYRO_LPR_PinST            BIT(1)    /*!< GPIO port B*/
#define GYRO_LPR_PinPD            BIT(5)    /*!< GPIO port B*/

#define GYRO_LY_ACD_ChZ           ADC_Channel_14
#define GYRO_LY_ACD_ChREF         ADC_Channel_15

#define GYRO_LPR_ACD_ChY          ADC_Channel_11
#define GYRO_LPR_ACD_ChX          ADC_Channel_10
#define GYRO_LPR_ACD_ChREF        ADC_Channel_12

//! Yaw axis Sensitivity [ mV/dps ] see LY330ALH datasheet
#define Gyro_Sensitivity_Y_300dps       3.752
//! Yaw axis Sensitivity [ LSB/dps]
#define Gyro_SensitivityLSB_Y_300dps     4.62

//! 300dps Full Scale Pitch axis Sensitivity [ mV/dps ]see LPR430AL datasheet
#define Gyro_Sensitivity_P_300dps        3.33
//! 300dps Full Scale Pitch axis Sensitivity [ LSB/dps ]
#define Gyro_SensitivityLSB_P_300dps     4.13

//! 300dps Full Scale  Roll axis Sensitivity [ mV/dps ]see LPR430AL datasheet
#define Gyro_Sensitivity_R_300dps        3.33
//! 300dps Full Scale Roll  axis Sensitivity [ LSB/dps ]
#define Gyro_SensitivityLSB_R_300dps     4.13

//! 1200 dps  full scale Roll axis Sensitivity [ mV/dps ]see LPR430AL datasheet
#define Gyro_Sensitivity_R_1200dps        0.83
//! 1200dps full scale Roll axis Sensitivity [ LSB/dps ]
#define Gyro_SensitivityLSB_R_1200dps     1.03

//! 1200 dps  full scale Pitch axis Sensitivity [ mV/dps ]see LPR430AL datasheet
#define Gyro_Sensitivity_P_1200dps        0.83
//! 1200 dps  full scale Pitch axis Sensitivity [ LSB/dps ]
#define Gyro_SensitivityLSB_P_1200dps     1.03


#ifdef HAS_GYRO
lpry_t gyro = {
	{0.0, 0.0, 0.0},
	0
};
#endif

void lpry_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef nvic_init_s;
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	RCC_APB2PeriphClockCmd( GYRO_RCC_Port |
	                        GYRO_GPIO_RCC_Port2 |
	                        GYRO_RCC_Periph_ADC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GYRO_LPR_PinY | GYRO_LPR_PinX | GYRO_LPR_PinREF | GYRO_LY_PinZ |GYRO_LY_PinREF;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GYRO_ADC_Port, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(GYRO_GPIO_RCC_Port2, ENABLE);
	
	// Configure control pins
	GPIO_InitStructure.GPIO_Pin = GYRO_LY_PinST | GYRO_LY_PinPD | GYRO_LPR_PinST |GYRO_LY_PinPD ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GYRO_GPIO_Port2, &GPIO_InitStructure);

	
	// GYRO_ADC configuration
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 5;
	
	// GYRO_ADC regular channel configuration
	ADC_RegularChannelConfig(GYRO_ADC, GYRO_LPR_ACD_ChX, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(GYRO_ADC, GYRO_LPR_ACD_ChY, 2,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(GYRO_ADC, GYRO_LPR_ACD_ChREF, 3,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(GYRO_ADC, GYRO_LY_ACD_ChZ, 4, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(GYRO_ADC, GYRO_LY_ACD_ChREF, 5,ADC_SampleTime_239Cycles5);
	
	ADC_Init(GYRO_ADC, &ADC_InitStructure);
	
	// Enable GYRO_ADC
	ADC_Cmd(GYRO_ADC, ENABLE);
	
	// Enable GYRO_ADC reset calibration register
	ADC_ResetCalibration(GYRO_ADC);
	// Check the end of GYRO_ADC reset calibration register
	while(ADC_GetResetCalibrationStatus(GYRO_ADC));
	
	// Start GYRO_ADC calibration
	ADC_StartCalibration(GYRO_ADC);
	// Check the end of GYRO_ADC calibration
	while(ADC_GetCalibrationStatus(GYRO_ADC));

	RCC_AHBPeriphClockCmd(GYRO_RCC_Periph_DMA, ENABLE);
	
	DMA_DeInit(GYRO_DMA_Channel);
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)GYRO_ADC_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)gyro.dma_buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 5;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(GYRO_DMA_Channel, &DMA_InitStructure);
	
	nvic_init_s.NVIC_IRQChannel = GYRO_DMA_IRQn;
	nvic_init_s.NVIC_IRQChannelPreemptionPriority = 2;
	nvic_init_s.NVIC_IRQChannelSubPriority = 2;
	nvic_init_s.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_init_s);
	
	DMA_ITConfig(GYRO_DMA_Channel, DMA_IT_TC, ENABLE);
	
	lpry_power_on();
	lpry_self_test_off();
}

void lpry_power_on(void){
	// Reset both power-down pins
	GYRO_GPIO_Port2->BRR = GYRO_LPR_PinPD | GYRO_LY_PinPD;
}

void lpry_power_off(void){
	// Set both power-down pins
	GYRO_GPIO_Port2->BSRR = GYRO_LY_PinPD | GYRO_LPR_PinPD;
}

void lpry_self_test_on(void){
	// Set both self-test pins
	GYRO_GPIO_Port2->BSRR = GYRO_LY_PinST | GYRO_LPR_PinST;

}

void lpry_self_test_off(void){
	// Reset both self-test pins
	GYRO_GPIO_Port2->BRR = GYRO_LY_PinST | GYRO_LPR_PinST;
}

void lpry_read(void){
	gyro.done = 0;
	ADC_DMACmd(GYRO_ADC, ENABLE);
	ADC_Cmd(GYRO_ADC, ENABLE);
	DMA_Cmd(GYRO_DMA_Channel, ENABLE);
	ADC_SoftwareStartConvCmd(GYRO_ADC, ENABLE);
}

int lpry_xfer_complete(void){
	if(gyro.done)
		return 1;
	return 0;
}

void lpry_update(void){
	gyro.reading.pitch =(gyro.dma_buffer[0] - gyro.dma_buffer[2]) / Gyro_SensitivityLSB_P_300dps;
	gyro.reading.roll =(gyro.dma_buffer[1] - gyro.dma_buffer[2]) / Gyro_SensitivityLSB_R_300dps;
	gyro.reading.yaw =(gyro.dma_buffer[3] - gyro.dma_buffer[4]) / Gyro_SensitivityLSB_Y_300dps;
	gyro.done = 0;
}

void lpry_read_sync(void){
	lpry_read();
	while(!lpry_xfer_complete());
	lpry_update();
}

void GYRO_DMA_ISR(void){
	if(DMA_GetFlagStatus(GYRO_DMA_FLAG)){
		ADC_Cmd(GYRO_ADC, DISABLE);
		ADC_SoftwareStartConvCmd(GYRO_ADC, DISABLE);
		DMA_ClearFlag(GYRO_DMA_FLAG);
		gyro.done = 1;
	}
}
