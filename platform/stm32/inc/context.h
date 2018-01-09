#ifndef CONTEXT_H
#define CONTEXT_H

#include "common.h"
#include "terminal.h"

#define PSR_DEFAULT 0x01000000

#define HW_CONTEXT_SIZE (sizeof(hw_context_frame) / HEAP_ALIGNMENT)
#define SW_CONTEXT_SIZE (sizeof(sw_context_frame) / HEAP_ALIGNMENT)

/**
 * stores registers saved by hardware
 */
typedef struct hw_context_frame_t {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t psr;
} __attribute__((packed)) hw_context_frame;

/**
 * stores registers saved by software
 */
typedef struct sw_context_frame_t {
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
} __attribute__((packed)) sw_context_frame;

void context_save(void);

void context_restore(void);

/**
 * @brief      pendSV exception handler. Performs context switch routine
 */
void pend_sv_handler(void);

/**
 * @brief      performs SV Call to swith to user mode
 */
void user_mode(void);

/**
 * @brief      performs initial stack initializion with hardware context
 *
 * @param      stack     stack base pointer
 * @param[in]  stk_size  stack size
 * @param[in]  task      working function
 */
void stack_setup(uint32_t *stack, int stk_size, void (*task)(void*));

/**
 * @brief      doing nothing...nuff said
 */
void idler(void);

/**
 * @brief      SV Call exception handler. Performs swith to user mode
 */
void sv_call_handler(void);

#endif
