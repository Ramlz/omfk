#include "led.h"
#include "gpio.h"
#include "clock.h"
#include "board_cfg.h"

void led_init(void) {
    gpio_init_pin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PUSH_PULL, GPIO_OUTPUT,
        GPIO_HIGH_SPEED, GPIO_NO_PUPD, NULL);
}

void led_loop(void) {
    bool light = false;
    while (true) {
        if (light) {
            gpio_low(LED_GPIO_PORT, LED_GPIO_PIN);
        } else {
            gpio_high(LED_GPIO_PORT, LED_GPIO_PIN);
        }
        light ^= 1;
        clock_dly_msecs(250);
    }
}
