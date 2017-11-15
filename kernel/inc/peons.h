#ifndef PEONS_H
#define PEONS_H

#include "common.h"
#include "context.h"
#include "memory.h"

/**
 * NOTE: stack and idler stack size are in heap alignment units
 */
#define PEON_STACK_SIZE 64
#define IDLER_STACK_SIZE 8

/**
 * @brief      just dirty hack to make compiler
 *             not warning about function pointers
 *
 */
#define TASK_PTR(task) ((void (*)())task)

/**
 * @brief      thread header (sircular single-liked list)
 */
typedef struct peon_t {
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
    uint32_t *context;
    /**
     * pointer to next thread in list
     */
    struct peon_t *next;
} __attribute__((packed, aligned(4))) peon;

/**
 * @brief      creates new thread
 *
 * @param[in]  task  pointer to working function
 */
void peon_create(void (*task)());

/**
 * @brief      TBD
 */
void peon_exterminate(void);

/**
 * @brief      initializes basic threads and enters user mode
 */
void peons_init(void);

/**
 * @brief      terminal command to output thread statistics
 */
void peon_stat(void);

#endif
