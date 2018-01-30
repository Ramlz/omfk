#include "context.h"

core_context *core_context_current = NULL;
core_context *core_context_scheduled = NULL;

void context_save(void) {
    register uint32_t *previous_context asm ("r0") __attribute__((unused)) =
        core_context_current->sw_frame;
    register void *previous_stack asm ("r1") __attribute__((unused)) =
        core_context_current->sp;

    asm volatile(
        //! store current sofware context
        "stm    r0!, {r4 - r11}              \n\t"
        //! store current stack pointer
        "mrs    r1, psp                      \n\t"
    );

    core_context_current->sp = (void*) previous_stack;
}

void context_restore(void) {
    register uint32_t *next_context asm ("r0") __attribute__((unused)) =
        core_context_scheduled->sw_frame;
    register void *next_stack asm ("r1") __attribute__((unused)) =
        core_context_scheduled->sp;
    asm volatile(
        //! restore scheduled sofware context
        "ldm      r0!, {r4 - r11}            \n\t"
        //! restore scheduled stack pointer
        "msr      psp, r1                    \n\t"
    );
}

void idler(void) {
    while (true) {
        asm volatile("nop");
    }
}

void stack_setup(uint32_t *stack, int stk_size, void (*task)(void*)) {
    stack[stk_size - 1] = PSR_DEFAULT;
    stack[stk_size - 2] = (uint32_t) task;
    stack[stk_size - 3] = (uint32_t) &idler;
}

void core_context_current_update(core_context *context) {
    core_context_current = context;
}

void core_context_scheduled_update(core_context *context) {
    core_context_scheduled = context;
}
