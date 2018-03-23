#ifndef PEONS_H
#define PEONS_H

#include "common/common.h"
#include "arch/context.h"
#include "kernel/memory.h"

/**
 * NOTE: stack and idler stack size are in heap alignment units
 */
#define PEON_STACK_SIZE 128
#define IDLER_STACK_SIZE 8

typedef enum peon_state_t {
    RUNNING      = 0,
    READY        = 1,
    STOPPED      = 2,
    LOCKED       = 3,
    EXTERMINATED = 4
} peon_state;

/**
 * @brief      thread header (sircular single-liked list)
 */
typedef struct peon_t {
    /**
     * stores saved CPU context
     */
    core_context context;
    /**
     * current thread state
     */
    peon_state status;
    /**
     * thread's name
     */
    char *name;
    /**
     * pointer to next thread in list
     */
    struct peon_t *next;
} __attribute__((packed, aligned(4))) peon;

/**
 * @brief      schedule thread for execution
 *
 * @return     another(true)/current(false) thread scheduled
 */
bool peons_schedule(void);

/**
 * @brief      creates new thread
 *
 * @param[in]  task  pointer to working function
 */
void peon_create(void (*task)(), char *name);

/**
 * @brief      TBD
 */
void peon_exterminate(void);

/**
 * @brief      initializes basic threads and enters user mode
 */
void peons_init(void);

/**
 * @brief      pauses thread by it's name
 *
 * @param      name  thread's name
 *
 * @return     error code
 */
void peon_stop_by_name(char *name);

/**
 * @brief      resumes thread by it's name
 *
 * @param      name  thread's name
 *
 * @return     error code
 */
void peon_resume_by_name(char *name);

/**
 * @brief      locks current thread
 */
void peon_lock(void);

/**
 * @brief      unlocks current thread
 */
void peon_unlock(void);

#endif
