#include "nvic.h"
#include "core.h"
#include "terminal.h"

extern uint32_t systick_msec;
extern core_context *core_context_current;

void nvic_it_enable(it_channel channel) {
    *((uint32_t*) ISER_BASE + ((uint8_t) channel >> 5)) = (0x1 << ((uint8_t) channel & 0x1f));
}

void nvic_it_disable(it_channel channel) {
    *((uint32_t*) ICER_BASE + ((uint8_t) channel >> 5)) = (0x1 << ((uint8_t) channel & 0x1f));
}

void isr_usart1(void) {
    if (receiver_available(USART_1)) {
        usart_write_buf(USART_1, get_char_unsafe(USART_1));
    }
}

void isr_usart2(void) {
    //! TODO
}

void default_handler(void) {
    terminal_error_message("unhandled interrupt!!!");
    while (true);
}

void h_fault_handler(uint32_t stack[]){
    terminal_error_message("CPU Hard Fault.");
    terminal_printf("SCB->HFSR      : 0x%x\r", HFSR);
    if ((HFSR & (1 << 30)) != 0) {
       terminal_error_message("Forced Hard Fault.");
       terminal_printf("SCB->CFSR      : 0x%x\r", CFSR);
       terminal_info_message("Register Dump  :");
       terminal_printf("r0  = 0x%x\r", stack[0]);
       terminal_printf("r1  = 0x%x\r", stack[1]);
       terminal_printf("r2  = 0x%x\r", stack[2]);
       terminal_printf("r3  = 0x%x\r", stack[3]);
       terminal_printf("r12 = 0x%x\r", stack[4]);
       terminal_printf("lr  = 0x%x\r", stack[5]);
       terminal_printf("pc  = 0x%x\r", stack[6]);
       terminal_printf("psr = 0x%x\r", stack[7]);
   }
    asm volatile("BKPT #01\n\t");
    while (true);
}

void pend_sv_call(void) {
    ICSR |= BIT28;
}

void systick_handler(void) {
    if (peons_schedule()) {
        context_save();
        pend_sv_call();
    }
    systick_msec++;
}

void pend_sv_handler(void) {
    context_restore();
}

void sv_call(sv_code svc_number) {
    register uint32_t svc_code asm ("r0") __attribute__((unused)) = svc_number;
    asm volatile (
        "svc 0"
    );
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
