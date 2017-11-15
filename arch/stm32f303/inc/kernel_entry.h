#ifndef KERNEL_ENTRY_H
#define KERNEL_ENTRY_H

#include "terminal.h"
#include "clock.h"
#include "usart.h"
#include "common.h"
#include "context.h"
#include "peons.h"

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
void h_fault_handler(void);

#endif
