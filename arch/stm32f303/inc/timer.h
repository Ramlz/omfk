#ifndef TIMER_H
#define TIMER_H

#include "arch.h"
/**
 * @brief      Initializes all timers used by kernel
 */
void timers_init(void);
/**
 * @brief      delay in useconds
 *
 * @param[in]  dly   useconds to wait
 */
void timer_tim1_dly_usec(uint16_t dly);
/**
 * @brief      delay in mseconds
 *
 * @param[in]  dly   mseconds to wait
 */
void timer_tim1_dly_msec(uint32_t dly);
/**
 * @brief      delay in seconds
 *
 * @param[in]  dly   seconds to wait
 */
void timer_tim1_dly_sec(uint32_t dly);

#endif
