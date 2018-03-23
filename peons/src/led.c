#include "peons/led.h"
#include "platform/gpio.h"
#include "platform/clock.h"
#include "board/cfg.h"

/**
 * GPIO interface with DHT pin
 */
static gpio_iface *led_gpio_iface = NULL;

/**
 * @brief      initializes GPIO pin connected to LED
 */
static void led_init(void) {
    //! create GPIO port interface
    led_gpio_iface = gpio_iface_get(LED_GPIO_PORT);

    //! init GPIO pin connected to led
    led_gpio_iface->init(led_gpio_iface, LED_GPIO_PIN, GPIO_OTYPE_PUSH_PULL,
        GPIO_MODE_OUTPUT, GPIO_SPEED_FAST, GPIO_PUPD_NO, NULL);
}

void led_loop(void) {
    led_init();
    while (true) {
        led_gpio_iface->toggle(led_gpio_iface, LED_GPIO_PIN);
        clock_dly_msecs(250);
    }
}
