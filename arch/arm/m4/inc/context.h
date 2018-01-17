#ifndef CONTEXT_H
#define CONTEXT_H

#include "common.h"

#define PSR_DEFAULT 0x01000000

#define HW_CONTEXT_SIZE (sizeof(hw_context_frame) / HEAP_ALIGNMENT)
#define SW_CONTEXT_SIZE (sizeof(sw_context_frame) / HEAP_ALIGNMENT)

/**
 * supervisor call codes enumeration
 */
typedef enum sv_code_t {
    /**
     * switch to user mode
     */
    SVC_USER_MODE   = 0,
    /**
     * switch to kernel mode
     */
    SVC_KERNEL_MODE = 1,
    /**
     * execute in privileged mode
     */
    SVC_EXECUTE     = 2
} sv_code;

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
 * @brief      pendSV exception handler. Performs context switch routine
 */
void pend_sv_handler(void);

/**
 * @brief      performs SV Call with given parameter
 * 
 * @param[in]  svc_number  number of supervisor function to call
 */
void sv_call(sv_code svc_number);

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
 * 
 * @param[in]  svc_args  supervisor call arguments
 */
void sv_call_handler(void);

/**
 * @brief      switch to user mode
 */
void sv_user_mode(void);

/**
 * @brief      switch to kernel mode
 */
void sv_kernel_mode(void);

void core_context_current_update(core_context *context);

void core_context_scheduled_update(core_context *context) ;

#endif
