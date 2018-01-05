#include "peons.h"

//! stack space used by idler thread
uint32_t idler_stack[IDLER_STACK_SIZE];

//! array for storing idlers software context
uint32_t idler_context[HW_CONTEXT_SIZE];

//! idler thread header (shouldn't be deleted)
static peon peon_idler = {
    .sp       = &idler_stack[IDLER_STACK_SIZE - HW_CONTEXT_SIZE],
    .sp_base  = idler_stack,
    .context  = idler_context,
    .status   = RUNNING,
    .next     = &peon_idler
};

//! current thread pointer
peon *peon_curr = &peon_idler;

//! current thread pointer
peon *peon_scheduled = &peon_idler;

bool peons_schedule(void) {
    peon *tmp = peon_curr;
    if (tmp->status != LOCKED) {
        tmp->status = READY;
        do {
            tmp = tmp->next;
        } while (tmp->status != READY);
    }
    tmp->status = RUNNING;
    peon_scheduled = tmp;
    return !(peon_curr == peon_scheduled);
}

void peon_lock(void) {
    peon_curr->status = LOCKED;
}

void peon_unlock(void) {
    peon_curr->status = RUNNING;
}

void peon_create(void (*task)()) {
    peon *curr = &peon_idler;
    //! find the last thread in list
    while (curr->next != &peon_idler) {
        curr = curr->next;
    }
    //! allocate new thread header
    peon *tmp = cell_alloc(sizeof(peon));
    curr->next = tmp;

    curr = curr->next;

    //! allocate stack for new thread
    uint32_t *stack = cell_alloc(PEON_STACK_SIZE * HEAP_ALIGNMENT);
    //! allocate space for storing software context
    uint32_t *context = cell_alloc(sizeof(hw_context_frame));

    curr->next = &peon_idler;
    curr->sp = &stack[PEON_STACK_SIZE - HW_CONTEXT_SIZE];
    curr->sp_base = stack;
    curr->context = context;
    curr->status = READY;

    //! setup initial hardware context on stack
    stack_setup(stack, PEON_STACK_SIZE, task);
}

void peon_exterminate(void) {
    //! TBD
}

void peons_init(void) {
    //! initialize idler thread stack
    stack_setup(idler_stack, IDLER_STACK_SIZE, TASK_PTR(idler));
}

void peon_stat(void) {
    peon *curr = &peon_idler;
    int cnt = 0;

    terminal_info_message("________________PEON INFO________________");

    terminal_printf("IDLER ADDR                   : 0x%X  ", curr);
    terminal_printf("IDLER STACK BASE             : 0x%X  ", curr->sp_base);
    terminal_printf("IDLER CURRENT STACK          : 0x%X\n", curr->sp);

    curr = curr->next;

    while (curr != &peon_idler) {
        terminal_printf("PEON %d:", cnt++);
        terminal_printf("    PEON ADDR                : 0x%X  ", curr);
        terminal_printf("    PEON STACK BASE          : 0x%X  ", curr->sp_base);
        terminal_printf("    PEON CURRENT STACK       : 0x%X\n", curr->sp);
        curr = curr->next;
    }
}
