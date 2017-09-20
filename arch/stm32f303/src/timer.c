#include "timer.h"

void init_timers(void) {
    // enable TIM6 clock
    RCC_APB1ENR |= BIT4;

    TIM6_CR1 |= BIT2;
    TIM6_CR1 |= BIT7;

    TIM6_CR2 &= ~BIT4;
    TIM6_CR2 |= BIT5;
    TIM6_CR2 &= ~BIT6;

    TIM6_DIER |= BIT8;

    // Counter frequency -> 8MHz / 80Hz = 100kHz
    TIM6_PSC = 80 - 1;

    TIM6_CR1 |= BIT0;
}

void update_tim6_rate(uint16_t new_rate_hz) {
    TIM6_ARR = (100000 / new_rate_hz);
}
