#ifndef KERNEL_ENTRY_H
#define KERNEL_ENTRY_H

#include "terminal.h"
#include "clock.h"
#include "usart.h"

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

/**
 * LD script externals
 */
extern unsigned char  INIT_DATA_VALUES;
extern unsigned char  INIT_DATA_START;
extern unsigned char  INIT_DATA_END;
extern unsigned char  BSS_START;
extern unsigned char  BSS_END;

#endif
