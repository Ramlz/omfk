#ifndef KERNEL_ENTRY_H
#define KERNEL_ENTRY_H

#include "terminal.h"
#include "clock.h"
#include "usart.h"
#include "common.h"
#include "context.h"
#include "peons.h"
#include "esp.h"

/**
 * @brief      Kernel main
 */
void kernel(void);

/**
 * @brief      Kernel entry point
 */
void kernel_entry(void);

/**
 * @brief      Handler for unused interrupts
 */
void default_handler(void);

/**
 * @brief      CPU hard fault handler
 */
void h_fault_handler(uint32_t stack[]);

/**
 * @defgroup USART_INTERRUPTS usart/uart interrupt routines
 * 
 * TODO: move it out of here
 *
 * @{
 */

void isr_usart1(void);

void isr_usart2(void);

/** @} */

#endif
