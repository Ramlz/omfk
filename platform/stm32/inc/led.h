#ifndef LED_H
#define LED_H

#define LED_GPIO_PORT GPIO_A
#define LED_GPIO_PIN 5

/**
 * @brief      initializes GPIO pin connected to LED
 */
void led_init(void);

/**
 * @brief      periodicaly turns LED on/off
 */
void led_loop(void);

#endif
