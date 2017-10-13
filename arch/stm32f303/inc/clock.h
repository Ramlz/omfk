#ifndef CLOCK_H
#define CLOCK_H

#include "stm32f303xx.h"

/**
 * @brief      gets the value of systick in seconds.
 *
 * @return     the systick seconds.
 */
uint64_t get_systick_seconds(void);

/**
 * @brief      gets the value of systick in miliseconds.
 *
 * @return     the systick miliseconds.
 */
uint16_t get_systick_miliseconds(void);
/**
 * @brief      delay produced by forcing cpu doing nothing
 *
 * @param[in]  dly   cycles to wait
 */
void dummy_delay(uint32_t dly);
/**
 * @brief      delay in miliseconds
 *
 * @param[in]  miliseconds  miliseconds to wait
 */
void clock_delay_miliseconds(uint32_t miliseconds);
/**
 * @brief      one second delay
 */
void clock_delay_second(void);
/**
 * @brief      delay in seconds
 *
 * @param[in]  seconds  seconds to wait
 */
void clock_delay_seconds(uint32_t seconds);
/**
 * @brief      snitialize systick
 */
void init_systick(void);
/**
 * @brief      initialize hardware clock/pripherial clock
 */
void init_clock(void);
/**
 * @brief      systick interrupt
 */
void systick(void);

#endif
