#include "context.h"
#include "peons.h"

extern peon *peon_curr;
extern peon *peon_scheduled;

void pend_sv_handler(void) {
    context_restore();

    //! update current thread pinter
    peon_curr = peon_curr->next;
}

void context_save(void) {
    register void *prev_cnt asm ("r0") __attribute__((unused)) =
        (void*) peon_curr->context;
    register void *prev_stk asm ("r1") __attribute__((unused)) =
        (void*) peon_curr->sp;

    asm volatile(
        //! store current sofware context
        "stm    r0!, {r4 - r11}  \n\t"
        //! store current stack pointer
        "mrs    r1, psp          \n\t"
    );

    peon_curr->sp = (void*) prev_stk;
}

void context_restore(void) {
    register void *next_cnt asm ("r0") __attribute__((unused)) =
        (void*) peon_scheduled->context;
    register void *next_stk asm ("r1") __attribute__((unused)) =
        (void*) peon_scheduled->sp;
    asm volatile(
        //! restore scheduled sofware context
        "ldm      r0!, {r4 - r11}\n\t"
        //! restore scheduled stack pointer
        "msr      psp, r1        \n\t"
    );
}

void user_mode(void) {
    asm volatile (
        "svc 0"
    );
}

void idler(void) {
    while(1) {
        asm volatile("nop");
    }
}

void stack_setup(uint32_t *stack, int stk_size, void (*task)(void*)) {
    stack[stk_size - 1] = PSR_DEFAULT;
    stack[stk_size - 2] = (uint32_t) task;
    stack[stk_size - 3] = (uint32_t) &idler;
}

void sv_call_handler(void) {
    register void *stack asm ("r0") __attribute__((unused)) =
        peon_curr->sp;
    asm volatile (
        "msr      psp, r0        \n\t"
        "mov      r0, $0x3       \n\t"
        "msr      control, r0    \n\t"
        "ldr      lr, =0xFFFFFFFD\n\t"
        "bx       lr             \n\t"
    );
}