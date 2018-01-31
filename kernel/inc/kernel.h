#ifndef KERNEL_H
#define KERNEL_H

#include "terminal.h"
#include "clock.h"
#include "memory.h"
#include "dht.h"
#include "context.h"
#include "peons.h"
#include "led.h"
#include "log.h"

/**
 * @brief      main kernel loop
 */
void kernel(void);

#endif
