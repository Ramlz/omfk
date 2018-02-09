#ifndef TIMER_H
#define TIMER_H

#include "peripheral.h"

typedef enum {
    TIM1  = TIM1_BASE,
    TIM2  = TIM2_BASE,
    TIM3  = TIM3_BASE,
    TIM4  = TIM4_BASE,
    TIM6  = TIM6_BASE,
    TIM7  = TIM7_BASE,
    TIM8  = TIM8_BASE,
    TIM15 = TIM15_BASE,
    TIM16 = TIM16_BASE,
    TIM17 = TIM17_BASE,
    TIM20 = TIM20_BASE,
} timer;

/**
 * @defgroup TIM_REGISTERS get address of TIM register
 * by base TIM address
 *
 * @{
 */

uint32_t *tim_cr1(uint32_t *base_addr);
uint32_t *tim_cr2(uint32_t *base_addr);
uint32_t *tim_cmsr(uint32_t *base_addr);
uint32_t *tim_dier(uint32_t *base_addr);
uint32_t *tim_sr(uint32_t *base_addr);
uint32_t *tim_egr(uint32_t *base_addr);
uint32_t *tim_ccmr1(uint32_t *base_addr);
uint32_t *tim_ccmr2(uint32_t *base_addr);
uint32_t *tim_ccer(uint32_t *base_addr);
uint32_t *tim_cnt(uint32_t *base_addr);
uint32_t *tim_psc(uint32_t *base_addr);
uint32_t *tim_rcr(uint32_t *base_addr);
uint32_t *tim_arr(uint32_t *base_addr);
uint32_t *tim_ccr1(uint32_t *base_addr);
uint32_t *tim_ccr2(uint32_t *base_addr);
uint32_t *tim_ccr3(uint32_t *base_addr);
uint32_t *tim_ccr4(uint32_t *base_addr);
uint32_t *tim_bdtr(uint32_t *base_addr);
uint32_t *tim_dcr(uint32_t *base_addr);
uint32_t *tim_dmar(uint32_t *base_addr);
uint32_t *tim_or(uint32_t *base_addr);
uint32_t *tim_ccmr3(uint32_t *base_addr);
uint32_t *tim_ccr5(uint32_t *base_addr);
uint32_t *tim_ccr6(uint32_t *base_addr);

/** @} */

/**
 * @brief      Initializes timer by its number
 */
void timer_init(timer timer_base_register);

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
