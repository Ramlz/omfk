#ifndef DMA_H
#define DMA_H

#include "stm32f303xx.h"
#include "dac.h"
#include "usart.h"
/**
 * @brief      Initializes DMA channels/enables it's clock
 */
void init_dma(void);

#endif