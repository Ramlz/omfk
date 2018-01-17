#include "peons.h"

//! stack space used by idler thread
uint32_t idler_stack[IDLER_STACK_SIZE];

//! array for storing idlers software context
uint32_t idler_context[HW_CONTEXT_SIZE];

//! idler thread header (shouldn't be deleted)
static peon peon_idler = {
    .context  = {
        .sp = &idler_stack[IDLER_STACK_SIZE - HW_CONTEXT_SIZE],
        .sp_base  = idler_stack,
        .sw_frame = idler_context,
    },
    .status   = RUNNING,
    .next     = &peon_idler
};

//! current thread pointer
static peon *peon_curr = &peon_idler;

bool peons_schedule(void) {
    peon *peon_scheduled = peon_curr;

    if (peon_curr->status != LOCKED) {
        //! current thread's no longer running
        peon_curr->status = READY;
        do {
            //! look for flexible thread
            peon_scheduled = peon_scheduled->next;
        } while (peon_scheduled->status != READY);
    }

    //! update scheduled thread status
    peon_scheduled->status = RUNNING;

    //! no need to switch context if scheduled and current are the same
    if (peon_curr == peon_scheduled) {
        return false;
    }
    //! handle old/new context for context switcher
    core_context_current_update(&peon_curr->context);
    core_context_scheduled_update(&peon_scheduled->context);

    //! update current thread pointer
    peon_curr = peon_scheduled;

    return true;
}

void peon_lock(void) {
    peon_curr->status = LOCKED;
}

void peon_unlock(void) {
    peon_curr->status = RUNNING;
}

void peon_create(void (*task)()) {
    peon *current = &peon_idler;
    //! find the last thread in list
    while (current->next != &peon_idler) {
        current = current->next;
    }
    //! allocate new thread header
    current->next = cell_alloc(sizeof(peon));

    current = current->next;

    //! allocate stack for new thread
    uint32_t *stack = cell_alloc(PEON_STACK_SIZE * HEAP_ALIGNMENT);
    //! allocate space for storing software context
    uint32_t *context = cell_alloc(sizeof(hw_context_frame));

    current->next = &peon_idler;
    current->context.sp = &stack[PEON_STACK_SIZE - HW_CONTEXT_SIZE];
    current->context.sp_base = stack;
    current->context.sw_frame = context;
    current->status = READY;

    //! setup initial hardware context on stack
    stack_setup(stack, PEON_STACK_SIZE, task);
}

void peon_exterminate(void) {
    //! TBD
}

void peons_init(void) {
    //! initialize idler thread stack
    stack_setup(idler_stack, IDLER_STACK_SIZE, TASK_PTR(idler));
    core_context_current_update(&peon_idler.context);
    core_context_scheduled_update(&peon_idler.context);
}

void peon_stat(void) {
    peon *curr = &peon_idler;
    int cnt = 0;

    terminal_info_message("________________PEON INFO________________");

    terminal_printf("IDLER ADDR                   : 0x%X  ", curr);
    terminal_printf("IDLER STACK BASE             : 0x%X  ",
        curr->context.sp_base);
    terminal_printf("IDLER CURRENT STACK          : 0x%X\n",
        curr->context.sp);

    curr = curr->next;

    while (curr != &peon_idler) {
        terminal_printf("PEON %d:", cnt++);
        terminal_printf("    PEON ADDR                : 0x%X  ",
            curr);
        terminal_printf("    PEON STACK BASE          : 0x%X  ",
            curr->context.sp_base);
        terminal_printf("    PEON CURRENT STACK       : 0x%X\n",
            curr->context.sp);
        curr = curr->next;
    }
}
