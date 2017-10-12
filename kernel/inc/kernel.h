#ifndef KERNEL_H
#define KERNEL_H

#include "terminal.h"
#include "clock.h"
#include "dac.h"
#include "gpio.h"
#include "timer.h"
#include "dma.h"
#include "memory.h"
/**
 * @brief      main kernel loop
 */
void kernel(void);

#endif
