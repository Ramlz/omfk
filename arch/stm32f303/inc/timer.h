#ifndef TIMER_H
#define TIMER_H

#include "arch.h"

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

volatile uint32_t *tim_cr1(volatile uint32_t *base_addr);

volatile uint32_t *tim_cr2(volatile uint32_t *base_addr);

volatile uint32_t *tim_cmsr(volatile uint32_t *base_addr);

volatile uint32_t *tim_dier(volatile uint32_t *base_addr);

volatile uint32_t *tim_sr(volatile uint32_t *base_addr);

volatile uint32_t *tim_egr(volatile uint32_t *base_addr);

volatile uint32_t *tim_ccmr1(volatile uint32_t *base_addr);

volatile uint32_t *tim_ccmr2(volatile uint32_t *base_addr);

volatile uint32_t *tim_ccer(volatile uint32_t *base_addr);

volatile uint32_t *tim_cnt(volatile uint32_t *base_addr);

volatile uint32_t *tim_psc(volatile uint32_t *base_addr);

volatile uint32_t *tim_rcr(volatile uint32_t *base_addr);

volatile uint32_t *tim_arr(volatile uint32_t *base_addr);

volatile uint32_t *tim_ccr1(volatile uint32_t *base_addr);

volatile uint32_t *tim_ccr2(volatile uint32_t *base_addr);

volatile uint32_t *tim_ccr3(volatile uint32_t *base_addr);

volatile uint32_t *tim_ccr4(volatile uint32_t *base_addr);

volatile uint32_t *tim_bdtr(volatile uint32_t *base_addr);

volatile uint32_t *tim_dcr(volatile uint32_t *base_addr);

volatile uint32_t *tim_dmar(volatile uint32_t *base_addr);

volatile uint32_t *tim_or(volatile uint32_t *base_addr);

volatile uint32_t *tim_ccmr3(volatile uint32_t *base_addr);

volatile uint32_t *tim_ccr5(volatile uint32_t *base_addr);

volatile uint32_t *tim_ccr6(volatile uint32_t *base_addr);

uint16_t timer_tim1_cnt_get(void);

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
