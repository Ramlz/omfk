#include "context.h"

static core_context *core_context_current = NULL;
static core_context *core_context_scheduled = NULL;

void pend_sv_handler(void) {
    context_restore();
}

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

void sv_call(sv_code svc_number) {
    register uint32_t svc_code asm ("r0") __attribute__((unused)) = svc_number;
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
    register uint32_t svc_code asm ("r0") __attribute__((unused));

    switch (svc_code) {
        case SVC_USER_MODE:
            //! return to user mode
            sv_user_mode();
            break;
        case SVC_KERNEL_MODE:
            //! return to kernel mode
            sv_kernel_mode();
            break;
        case SVC_EXECUTE:
            //! TBD
            break;
        default:
            break;
    }
}

void sv_user_mode(void) {
    register void *user_stack asm ("r0") __attribute__((unused)) =
        core_context_current->sp;
    asm volatile (
        //! update PSP from current user thread's SP
        "msr      psp, r0                    \n\t"
        //! can't update CONTROL register directly: using intermedite register
        "mov      r0, $0x3                   \n\t"
        //! CONTROL BITS:
        //! BIT_0 -> on  :  unprivileged mode
        //! BIT_1 -> on  :  pusing PSP
        //! BIT_2 -> off :  not using FPU context
        "msr      control, r0                \n\t"
        //! EXC_RETURN : thread mode with PSP
        "ldr      lr, =0xFFFFFFFD            \n\t"
        //! branch to new mode
        "bx       lr                         \n\t"
    );
}

void sv_kernel_mode(void) {
    asm volatile (
        //! can't update CONTROL register directly: using intermedite register
        "mov      r0, $0x0                   \n\t"
        //! CONTROL BITS:
        //! BIT_0 -> off :  privileged mode
        //! BIT_1 -> off :  pusing MSP
        //! BIT_2 -> off :  not using FPU context
        "msr      control, r0                \n\t"
        //! EXC_RETURN : thread mode with MSP
        "ldr      lr, =0xFFFFFFF9            \n\t"
        //! branch to new mode
        "bx       lr                         \n\t"
    );
}

void core_context_current_update(core_context *context) {
    core_context_current = context;
}

void core_context_scheduled_update(core_context *context) {
    core_context_scheduled = context;
}
