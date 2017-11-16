#ifndef KERNEL_H
#define KERNEL_H

#include "terminal.h"
#include "clock.h"
#include "memory.h"
#include "timer.h"
#include "dht.h"
#include "context.h"
#include "peons.h"

/**
 * @brief      main kernel loop
 */
void kernel(void);

/**
 * @brief      periodicaly read DHT sensor data
 */
void peon_dht(void);

#endif
