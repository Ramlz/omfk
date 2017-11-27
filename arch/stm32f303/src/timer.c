#include "timer.h"

volatile uint32_t *tim_cr1(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 0);
}

volatile uint32_t *tim_cr2(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 1);
}

volatile uint32_t *tim_cmsr(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 2);
}

volatile uint32_t *tim_dier(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 3);
}

volatile uint32_t *tim_sr(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 4);
}

volatile uint32_t *tim_egr(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 5);
}

volatile uint32_t *tim_ccmr1(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 6);
}

volatile uint32_t *tim_ccmr2(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 7);
}

volatile uint32_t *tim_ccer(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 8);
}

volatile uint32_t *tim_cnt(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 9);
}

volatile uint32_t *tim_psc(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 10);
}

volatile uint32_t *tim_rcr(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 11);
}

volatile uint32_t *tim_arr(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 12);
}

volatile uint32_t *tim_ccr1(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 13);
}

volatile uint32_t *tim_ccr2(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 14);
}

volatile uint32_t *tim_ccr3(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 15);
}

volatile uint32_t *tim_ccr4(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 16);
}

volatile uint32_t *tim_bdtr(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 17);
}

volatile uint32_t *tim_dcr(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 18);
}

volatile uint32_t *tim_dmar(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 19);
}

volatile uint32_t *tim_or(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 20);
}

volatile uint32_t *tim_ccmr3(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 21);
}

volatile uint32_t *tim_ccr5(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 22);
}

volatile uint32_t *tim_ccr6(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 23);
}


void timer_init(timer timer_base_register) {
    volatile uint32_t *tim_addr = (volatile uint32_t*) timer_base_register;
    switch (timer_base_register) {
        case TIM1:
            // turn on timer clock if not yet enabled
            if (!(RCC_APB2ENR & BIT11)) {
                RCC_APB2ENR   |= BIT11;
            }
            *tim_cr1(tim_addr)  |= BIT7;
            *tim_psc(tim_addr)   = 0x20;
            *tim_arr(tim_addr)   = 0x02;
            *tim_cnt(tim_addr)   = 0x00;
            *tim_dier(tim_addr) |= BIT0;
            *tim_cr1(tim_addr)  |= BIT0;
            break;
        case TIM2:
            // turn on timer clock if not yet enabled
            if (!(RCC_APB1ENR & BIT0)) {
                RCC_APB1ENR   |= BIT0;
            }
            break;
        case TIM3:
            // turn on timer clock if not yet enabled
            if (!(RCC_APB1ENR & BIT1)) {
                RCC_APB1ENR   |= BIT1;
            }
            break;
        case TIM4:
            // turn on timer clock if not yet enabled
            if (!(RCC_APB1ENR & BIT2)) {
                RCC_APB1ENR   |= BIT2;
            }
            break;
        case TIM6:
            // turn on timer clock if not yet enabled
            if (!(RCC_APB1ENR & BIT4)) {
                RCC_APB1ENR   |= BIT4;
            }
            break;
        case TIM7:
            // turn on timer clock if not yet enabled
            if (!(RCC_APB1ENR & BIT5)) {
                RCC_APB1ENR   |= BIT5;
            }
            break;
        case TIM8:
            // turn on timer clock if not yet enabled
            if (!(RCC_APB2ENR & BIT13)) {
                RCC_APB2ENR   |= BIT13;
            }
            break;
        case TIM15:
            // turn on timer clock if not yet enabled
            if (!(RCC_APB2ENR & BIT16)) {
                RCC_APB2ENR   |= BIT16;
            }
        case TIM16:
            // turn on timer clock if not yet enabled
            if (!(RCC_APB2ENR & BIT17)) {
                RCC_APB2ENR   |= BIT17;
            }
        case TIM17:
            // turn on timer clock if not yet enabled
            if (!(RCC_APB2ENR & BIT18)) {
                RCC_APB2ENR   |= BIT18;
            }
        case TIM20:
            // turn on timer clock if not yet enabled
            if (!(RCC_APB2ENR & BIT20)) {
                RCC_APB2ENR   |= BIT20;
            }
        default:
            return;
    }
}

void timer_tim1_dly_usec(uint16_t dly) {
    *tim_cnt((volatile uint32_t*) TIM1) = 0;
    uint16_t saved_usecs = *tim_cnt((volatile uint32_t*) TIM1) + dly;
    while (saved_usecs > *tim_cnt((volatile uint32_t*) TIM1));
}

uint16_t timer_tim1_cnt_get(void) {
    return *tim_cnt((volatile uint32_t*) TIM1);
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
