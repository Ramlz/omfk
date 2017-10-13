#include "clock.h"

volatile static uint16_t systick_miliseconds= 0;
volatile static uint64_t systick_seconds = 0;

uint64_t get_systick_seconds(void) {
    return systick_seconds;
}

uint16_t get_systick_miliseconds(void) {
    return systick_miliseconds;
}

void dummy_delay(uint32_t dly) {
    while(dly--);
}

void clock_delay_miliseconds(uint32_t miliseconds) {
    uint64_t saved_miliseconds = systick_miliseconds + miliseconds;
    while (saved_miliseconds > systick_miliseconds);
}

void clock_delay_second(void) {
    uint16_t saved_miliseconds = systick_miliseconds;
    while(saved_miliseconds == systick_miliseconds);
    while(saved_miliseconds != systick_miliseconds);
}

void clock_delay_seconds(uint32_t seconds) {
    while (seconds--) {
        clock_delay_second();
    }
}

void init_systick() {
    STK_CSR |= ( BIT2 | BIT1 | BIT0);
    // set systick prescaler
    // 64MHz / 64kHz = 1kHz
    STK_RVR = 0xFA00;
    STK_CVR = 10;
    systick_miliseconds = 0;
}

void systick(void) {
    systick_miliseconds++;
    if (systick_miliseconds > 1000) {
        systick_miliseconds = 0;
        systick_seconds++;
    }
}

void init_clock(void) {
    // Using HSI clock for PLL (8MHz)

    // turn off PLL
    RCC_CR &= ~BIT24;
    dummy_delay(10);
    // PLL multiply = 16 -> HCLK = 64MHz
    RCC_CFGR |= ((0xFF) << 18);
    dummy_delay(10);
    // APB1/APB2 clock speed = HCLK / 2 = 32MHz
    RCC_CFGR |= BIT10;
    RCC_CFGR |= BIT13;
    dummy_delay(10);
    // turn on PLL
    RCC_CR |= BIT24;
    dummy_delay(10);
    // set PLL as clock
    RCC_CFGR |= BIT1;
    // two wait states
    // for flash access
    // 48 < HCLK <= 72 MHz
    FLASH_ACR |= BIT1;
}
