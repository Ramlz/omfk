#include "clock.h"

volatile static uint32_t systick_msec = 0;

uint32_t clock_get(void) {
    return systick_msec;
}

void dummy_dly(uint32_t dly) {
    while(dly--);
}

void clock_dly_msecs(uint32_t msecs) {
    uint32_t saved_msecs = systick_msec + msecs;
    while (saved_msecs > systick_msec);
}

void clock_dly_secs(uint32_t secs) {
    while (secs--) {
        clock_dly_msecs(1000);
    }
}

void systick_init(void) {
    STK_CSR |= ( BIT2 | BIT1 | BIT0);
    // set systick prescaler
    // 64MHz / 64kHz = 1kHz (every 1 msecond)
    STK_RVR = 0xFA00;
    STK_CVR = 1000;

    // high priority for systick exception
    SHPR3 |= (0x0f << 24);
    // the lowest possible priority for PendSV exception
    SHPR3 |= (0xff << 16);
}

void systick_handler(void) {
    systick_msec++;
    if(!context_locked()) {
        pend_sv_call();
    }
}

void clock_init(void) {
    // Using HSI clock for PLL (8MHz)

    // turn off PLL
    RCC_CR &= ~BIT24;
    dummy_dly(10);
    // PLL multiply = 16 -> HCLK = 64MHz
    RCC_CFGR |= ((0xFF) << 18);
    dummy_dly(10);
    // APB1/APB2 clock speed = HCLK / 2 = 32MHz
    RCC_CFGR |= BIT10;
    RCC_CFGR |= BIT13;
    dummy_dly(10);
    // turn on PLL
    RCC_CR |= BIT24;
    dummy_dly(10);
    // set PLL as clock
    RCC_CFGR |= BIT1;
    // two wait states
    // for flash access
    // 48 < HCLK <= 72 MHz
    FLASH_ACR |= BIT1;
}

void pend_sv_call(void) {
    ICSR |= BIT28;
}
