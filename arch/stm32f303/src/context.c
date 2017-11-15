#include "context.h"

#include "peons.h"

extern peon *peon_curr;

void pend_sv_handler(void) {
    // move pointers of contexts in registers
    register uint32_t *prev_cnt asm ("r0") = peon_curr->context;
    register uint32_t *next_cnt asm ("r1") = peon_curr->next->context;

    asm volatile(
        // store sofware context of old thread
        "stm    r0!, {r4 - r11}  \n\t"
        // restore context of new thread
        "ldm    r1!, {r4 - r11}  \n\t"
    );

    // move stack pointers to registers
    register uint32_t *prev_stk asm ("r2") = peon_curr->sp;
    register uint32_t *next_stk asm ("r3") = peon_curr->next->sp;

    // update stack pointer
    asm volatile(
        "mrs      r2, psp        \n\t"
        "msr      psp, r3        \n\t"
    );

    // save previous stack pointer
    peon_curr->sp = prev_stk;

    // update current thread pinter
    peon_curr = peon_curr->next;
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
    register uint32_t *prev asm ("r3")  = peon_curr->sp;
    asm volatile (
        "msr      psp, r3        \n\t"
        "mov      r3, $0x3       \n\t"
        "msr      control, r3    \n\t"
        "ldr      lr, =0xFFFFFFFD\n\t"
        "bx       lr             \n\t"
    );
}
