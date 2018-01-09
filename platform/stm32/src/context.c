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
    register uint32_t *prev_cnt asm ("r0") = peon_curr->context;
    register uint32_t *prev_stk asm ("r1") = peon_curr->sp;

    asm volatile(
        //! store current sofware context
        "stm    r0!, {r4 - r11}  \n\t"
        //! store current stack pointer
        "mrs    r1, psp          \n\t"
    );

    peon_curr->sp = prev_stk;
}

void context_restore(void) {
    register uint32_t *next_cnt asm ("r0") = peon_scheduled->context;
    register uint32_t *next_stk asm ("r1") = peon_scheduled->sp;
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
    stack[stk_size - 2] = (uint32_t)task;
    stack[stk_size - 3] = (uint32_t)&idler;
}

void sv_call_handler(void) {
    register uint32_t *prev asm ("r0")  = peon_curr->sp;
    asm volatile (
        "msr      psp, r0        \n\t"
        "mov      r0, $0x3       \n\t"
        "msr      control, r0    \n\t"
        "ldr      lr, =0xFFFFFFFD\n\t"
        "bx       lr             \n\t"
    );
}