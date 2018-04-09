#ifndef PEON_LED_H
#define PEON_LED_H

#include "common/common.h"

/**
 * @brief      periodicaly turns LED on/off
 */
void led_loop(void);

/**
 * @brief      enable/disable LED blinking
 *
 * @param[in]  enable  enable/disable flag
 */
void led_enable(bool enable);

/**
 * @brief      set interval of LED blinking (msecs)
 *
 * @param[in]  interval  interval of blinking
 */
void led_set_interval(uint32_t interval);

#endif
