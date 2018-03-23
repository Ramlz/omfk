#ifndef TIMER_H
#define TIMER_H

#include "platform/peripheral.h"
#include "common/common.h"

/**
 * timer base registerss enumeration
 */
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
 * time values prescalers
 */
typedef enum timer_time_value_t {
    TIMER_MICRO_SECOND = 1,
    TIMER_MILI_SECOND  = 1000,
    TIMER_SECOND       = 1000000
} timer_time_value;

/**
 * timer conuter modes
 */
typedef enum timer_counter_mode_t {
    TIMER_COUNTER_MODE_UP               = 0x0000,
    TIMER_COUNTER_MODE_DOWN             = 0x0010,
    TIMER_COUNTER_MODE_CENTER_ALIGNED_1 = 0x0020,
    TIMER_COUNTER_MODE_CENTER_ALIGNED_2 = 0x0040,
    TIMER_COUNTER_MODE_CENTER_ALIGNED_3 = 0x0060
} timer_counter_mode;

/**
 * configuration used during timer initialization
 */
typedef struct timer_config_t {
    timer_counter_mode   counter_mode;
    uint16_t             prescaler;
    uint32_t             period;
} timer_config;

/**
 * generic timer interface sstruct
 */
typedef struct timer_iface_t {
    /**
     * @brief      wait until timer counts to value
     *
     * @param      iface        timer interface
     * @param[in]  time_value   value multiplier
     * @param[in]  time_points  time value to wait
     */
    void (*delay)(struct timer_iface_t *iface, timer_time_value time_value,
        uint32_t time_points);
    /**
     * @brief      get current value of timer counter
     *
     * @param      iface  timer interface
     *
     * @return     counter value
     */
    uint32_t (*get_counter)(struct timer_iface_t *iface);
    /**
     * @brief      enable/disable timer
     *
     * @param      iface    timer interface
     * @param[in]  enabled  enable/disable flag
     */
    void (*enable)(struct timer_iface_t *iface, bool enabled);
    /**
     * @brief      free allocated for timer memory
     *
     * @param      iface  timer interface
     */
    void (*destroy)(struct timer_iface_t *iface);
} timer_iface;

/**
 * @brief      get desired timer interface
 *
 * @param[in]  timer_base  desired timer
 *
 * @return     timer interface if it's initialized, otherwise NULL
 */
timer_iface *timer_iface_get(timer timer_base);

/**
 * @brief      initialize timer
 *
 * @param[in]  timer_base  desired timer
 * @param[in]  config      timer configuration (mandatory)
 */
void timer_init(timer timer_base, const timer_config *config);

#endif
