#ifndef LED_H
#define LED_H

/**
 * @brief      initializes GPIO pin connected to LED
 */
void led_init(void);

/**
 * @brief      periodicaly turns LED on/off
 */
void led_loop(void);

#endif
