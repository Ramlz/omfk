#ifndef CLOCK_H
#define CLOCK_H

#include "stm32f303xx.h"

/**
 * @brief      Gets the value of systick in seconds.
 *
 * @return     The systick seconds.
 */
uint64_t get_systick_seconds(void);

/**
 * @brief      Gets the value of systick in miliseconds.
 *
 * @return     The systick miliseconds.
 */
uint16_t get_systick_miliseconds(void);
/**
 * @brief      Delay produced by forcing CPU doing nothing
 *
 * @param[in]  dly   The dly
 */
void dummy_delay(uint32_t dly);
/**
 * @brief      one second delay
 */
void clock_delay_second(void);
/**
 * @brief      parametrized delay
 *
 * @param[in]  seconds  Seconds to wait
 */
void clock_delay_seconds(uint32_t seconds);
/**
 * @brief      Initialize systick
 */
void init_systick(void);
/**
 * @brief      Initialize hardware clock/pripherial clock
 */
void init_clock(void);
/**
 * @brief      Systick interrupt
 */
void systick(void);

#endif
