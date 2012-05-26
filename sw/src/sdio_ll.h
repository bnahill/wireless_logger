#ifndef __SDIO_LL_H_
#define __SDIO_LL_H_

#include "stm32f10x.h"
#include "sensor_config.h"
#include "sdio.h"

#define SDIO_INIT_CLK_DIV ((uint8_t)0xB2)
#define SDIO_TRANSFER_CLK_DIV ((uint8_t)0x01)

#define SDIO_FIFO_Address               ((u32)0x40018080)

void SD_LowLevel_Init(void);
void SD_LowLevel_DeInit(void);
void SD_LowLevel_DMA_RxConfig(uint32_t *dst, uint32_t cnt);
void SD_LowLevel_DMA_TxConfig(const uint32_t *src, uint32_t cnt);

FlagStatus SD_DMAEndOfTransferStatus(void);

#endif
