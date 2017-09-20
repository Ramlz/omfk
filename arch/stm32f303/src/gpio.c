#include "gpio.h"

void init_gpio(void) {
    // turn on GPIOA clock
    RCC_AHBENR  |= BIT17;

    // set alternate function mode
    // for usart2 pins (PA2->tx, PA3->rx)
    GPIOA_MODER |= (BIT5 | BIT7);
    GPIOA_MODER &= ~(BIT6 | BIT4);

    // alternate function AF7
    // for usart2 pins
    GPIOA_AFRL  |= (BIT12 | BIT13 | BIT14);
    GPIOA_AFRL  |= (BIT8 | BIT9 | BIT10);
    GPIOA_AFRL  &= ~(BIT15 | BIT11);

    // analog mode for dac1
    // output (PA4)
    GPIOA_MODER |= (BIT8 | BIT9);
}
