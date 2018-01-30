#ifndef CONTEXT_H
#define CONTEXT_H

#include "common.h"

#define PSR_DEFAULT 0x01000000

#define HW_CONTEXT_SIZE (sizeof(hw_context_frame) / HEAP_ALIGNMENT)
#define SW_CONTEXT_SIZE (sizeof(sw_context_frame) / HEAP_ALIGNMENT)

#define FA_START()                                               \
    register void *reg0 asm ("r0") __attribute__((unused));   \
    register void *reg1 asm ("r1") __attribute__((unused));   \
    register void *reg2 asm ("r2") __attribute__((unused));   \
    register void *reg3 asm ("r3") __attribute__((unused));   \
    register void *stk_arg asm ("r4") __attribute__((unused));\

#define FA_HANDLE(ARGUMENT_NUMBER, ARGUMENT)                     \
    switch(ARGUMENT_NUMBER) {                                    \
        case 0:                                                  \
            reg0 = (void*) ARGUMENT;                             \
            break;                                               \
        case 1:                                                  \
            reg1 = (void*) ARGUMENT;                             \
            break;                                               \
        case 2:                                                  \
            reg2 = (void*) ARGUMENT;                             \
            break;                                               \
        case 3:                                                  \
            reg3 = (void*) ARGUMENT;                             \
            break;                                               \
        default:                                                 \
            stk_arg = (void*) ARGUMENT;                          \
            asm volatile(                                        \
                "push     {r4}                       \n\t"       \
            );                                                   \
            break;                                               \
    }

/**
 * core context to be stored in thread's header
 */
typedef struct core_context_t {
    /**
     * saved stack pointer
     */
    void *sp;
    /**
     * base of allocated stack space
     */
    void *sp_base;
    /**
     * pointer to saved software context
     */
    uint32_t *sw_frame;
} __attribute__((packed)) core_context;

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

/**
 * @brief      saves current CPU context into thread's header
 */
void context_save(void);

/**
 * @brief      restores CPU context from thread's header
 */
void context_restore(void);

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
 * @brief      update curent thread's context holder address
 *
 * @param      context  context holder ptr
 */
void core_context_current_update(core_context *context);

/**
 * @brief      update scheduled thread's context holder address
 *
 * @param      context  context holder ptr
 */
void core_context_scheduled_update(core_context *context) ;

#endif
