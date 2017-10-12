#ifndef KERNEL_ENTRY_H
#define KERNEL_ENTRY_H

#include "terminal.h"
#include "clock.h"
#include "usart.h"
#include "common.h"

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
void hard_fault_handler(void);

#endif
