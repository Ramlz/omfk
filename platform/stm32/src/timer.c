#include "timer.h"

uint32_t *tim_cr1(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 0);
}

uint32_t *tim_cr2(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 1);
}

uint32_t *tim_cmsr(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 2);
}

uint32_t *tim_dier(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 3);
}

uint32_t *tim_sr(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 4);
}

uint32_t *tim_egr(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 5);
}

uint32_t *tim_ccmr1(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 6);
}

uint32_t *tim_ccmr2(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 7);
}

uint32_t *tim_ccer(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 8);
}

uint32_t *tim_cnt(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 9);
}

uint32_t *tim_psc(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 10);
}

uint32_t *tim_rcr(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 11);
}

uint32_t *tim_arr(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 12);
}

uint32_t *tim_ccr1(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 13);
}

uint32_t *tim_ccr2(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 14);
}

uint32_t *tim_ccr3(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 15);
}

uint32_t *tim_ccr4(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 16);
}

uint32_t *tim_bdtr(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 17);
}

uint32_t *tim_dcr(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 18);
}

uint32_t *tim_dmar(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 19);
}

uint32_t *tim_or(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 20);
}

uint32_t *tim_ccmr3(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 21);
}

uint32_t *tim_ccr5(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 22);
}

uint32_t *tim_ccr6(uint32_t *base_addr) {
    return (uint32_t*) (base_addr + 23);
}


void timer_init(timer timer_base_register) {
    uint32_t *tim_addr = (uint32_t*) timer_base_register;
    switch (timer_base_register) {
        case TIM1:
            //! turn on timer clock if not yet enabled
            if (!(RCC_APB2ENR & BIT11)) {
                RCC_APB2ENR   |= BIT11;
            }
            *tim_cr1(tim_addr)  |= BIT7;
            *tim_psc(tim_addr)   = 0x10;
            *tim_arr(tim_addr)   = 0x02;
            *tim_cnt(tim_addr)   = 0x00;
            *tim_dier(tim_addr) |= BIT0;
            *tim_cr1(tim_addr)  |= BIT0;
            break;
        case TIM2:
            //! turn on timer clock if not yet enabled
            if (!(RCC_APB1ENR & BIT0)) {
                RCC_APB1ENR   |= BIT0;
            }
            break;
        case TIM3:
            //! turn on timer clock if not yet enabled
            if (!(RCC_APB1ENR & BIT1)) {
                RCC_APB1ENR   |= BIT1;
            }
            break;
        case TIM4:
            //! turn on timer clock if not yet enabled
            if (!(RCC_APB1ENR & BIT2)) {
                RCC_APB1ENR   |= BIT2;
            }
            break;
        case TIM6:
            //! turn on timer clock if not yet enabled
            if (!(RCC_APB1ENR & BIT4)) {
                RCC_APB1ENR   |= BIT4;
            }
            break;
        case TIM7:
            //! turn on timer clock if not yet enabled
            if (!(RCC_APB1ENR & BIT5)) {
                RCC_APB1ENR   |= BIT5;
            }
            break;
        case TIM8:
            //! turn on timer clock if not yet enabled
            if (!(RCC_APB2ENR & BIT13)) {
                RCC_APB2ENR   |= BIT13;
            }
            break;
        case TIM15:
            //! turn on timer clock if not yet enabled
            if (!(RCC_APB2ENR & BIT16)) {
                RCC_APB2ENR   |= BIT16;
            }
            break;
        case TIM16:
            //! turn on timer clock if not yet enabled
            if (!(RCC_APB2ENR & BIT17)) {
                RCC_APB2ENR   |= BIT17;
            }
            break;
        case TIM17:
            //! turn on timer clock if not yet enabled
            if (!(RCC_APB2ENR & BIT18)) {
                RCC_APB2ENR   |= BIT18;
            }
            break;
        case TIM20:
            //! turn on timer clock if not yet enabled
            if (!(RCC_APB2ENR & BIT20)) {
                RCC_APB2ENR   |= BIT20;
            }
            break;
        default:
            break;
    }
}

void timer_tim1_dly_usec(uint16_t dly) {
    *tim_cnt((uint32_t*) TIM1) = 0;
    while (dly > *tim_cnt((uint32_t*) TIM1));
}

void timer_tim1_dly_msec(uint32_t dly) {
    while (dly--) {
        timer_tim1_dly_usec(1000);
    }
}

void timer_tim1_dly_sec(uint32_t dly) {
    while (dly--) {
        timer_tim1_dly_msec(1000);
    }
}
