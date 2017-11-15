#ifndef CLOCK_H
#define CLOCK_H

#include "arch.h"
#include "context.h"
#include "peons.h"

/**
 * @brief      gets the value of systick in mseconds.
 *
 * @return     the systick mseconds.
 */
uint32_t clock_get(void);
/**
 * @brief      delay produced by forcing cpu doing nothing
 *
 * @param[in]  dly   cycles to wait
 */
void dummy_delay(uint32_t dly);
/**
 * @brief      delay in mseconds
 *
 * @param[in]  msecs  msecconds to wait
 */
void clock_dly_msecs(uint32_t msecs);
/**
 * @brief      delay in seconds
 *
 * @param[in]  secs  secconds to wait
 */
void clock_dly_secs(uint32_t secs);
/**
 * @brief      snitialize systick
 */
void systick_init(void);
/**
 * @brief      initialize hardware clock/pripherial clock
 */
void clock_init(void);
/**
 * @brief      systick interrupt
 */
void systick_handler(void);
/**
 * @brief      pends execution of pend_sv_handler (used by systick handler)
 */
void pend_sv_call(void);

#endif
