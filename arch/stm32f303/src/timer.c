#include "timer.h"
#include "terminal.h"

void timers_init(void) {
    /**
     * @defgroup TIM1_INIT initialization of
     * general purpose timer TIM1 to count up every usecond
     *
     * @{
     */

    RCC_APB2ENR |= BIT11;

    TIM1_CR1 |= BIT7;

    TIM1_PSC = 0x40;

    TIM1_ARR = 0xffff;

    TIM1_CNT = 0x0;

    TIM1_CR1 |= BIT0;

    /** @} */
}

void timer_tim1_dly_usec(uint16_t dly) {
    TIM1_CNT = 0;
    uint16_t saved_usecs = TIM1_CNT + dly;
    while (saved_usecs > TIM1_CNT);
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
