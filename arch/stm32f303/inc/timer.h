#ifndef TIMER_H
#define TIMER_H

#include "stm32f303xx.h"
/**
 * @brief      Initializes all timers used by kernel
 */
void init_timers(void);
/**
 * @brief      TIM6 rate on-the-fly updater
 *
 * @param[in]  new_rate_hz  The new rate hz
 */
void update_tim6_rate(uint16_t new_rate_hz);

#endif
