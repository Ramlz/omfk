#ifndef NVIC_H
#define NVIC_H

#include "common/common.h"

typedef enum it_channel_t {
    IT_WWDG         = 0,
    IT_PVD          = 1,
    IT_TAMPER_STAMP = 2,
    IT_RTC_WKUP     = 3,
    IT_FLASH        = 4,
    IT_RCC          = 5,
    IT_EXTI0        = 6,
    IT_EXTI1        = 7,
    IT_EXTI2_TS     = 8,
    IT_EXTI3        = 9,
    IT_EXTI4        = 10,
    IT_DMA1_CH1     = 11,
    IT_DMA1_CH2     = 12,
    IT_DMA1_CH3     = 13,
    IT_DMA1_CH4     = 14,
    IT_DMA1_CH5     = 15,
    IT_DMA1_CH6     = 16,
    IT_DMA1_CH7     = 17,
    IT_ADC1_2       = 18,
    IT_CAN_TX       = 19,
    IT_CAN_RX0      = 20,
    IT_CAN_RX1      = 21,
    IT_CAN_SCE      = 22,
    IT_EXTI9_5      = 23,
    IT_TIM1_BRK     = 24,
    IT_TIM1_UP      = 25,
    IT_TIM1_TRG_COM = 26,
    IT_TIM1_CC      = 27,
    IT_TIM2         = 28,
    IT_TIM3         = 29,
    IT_TIM4         = 30,
    IT_I2C1_EV      = 31,
    IT_I2C1_ER      = 32,
    IT_I2C2_EV      = 33,
    IT_I2C2_ER      = 34,
    IT_SPI1         = 35,
    IT_SPI2         = 36,
    IT_USART1       = 37,
    IT_USART2       = 38,
    IT_USART3       = 39,
    IT_EXTI15_10    = 40,
    IT_RTC_ALARM    = 41,
    IT_EXTI19       = 42,
    IT_TIM8_BRK     = 43,
    IT_TIM8_UP      = 44,
    IT_TIM8_TRG_COM = 45,
    IT_TIM8_CC      = 46,
    IT_ADC3         = 47,
    /**
     * RESERVED [48-50]
     */
    IT_SPI3         = 51,
    IT_UART4        = 52,
    IT_UART5        = 53,
    IT_TIM6_DAC     = 54,
    IT_TIM7         = 55,
    IT_DMA2_CH1     = 56,
    IT_DMA2_CH2     = 57,
    IT_DMA2_CH3     = 58,
    IT_DMA2_CH4     = 59,
    IT_DMA2_CH5     = 60,
    IT_ADC4         = 61,
    /**
     * RESERVED [62-63]
     */
    IT_COMP1_2_3    = 64,
    IT_COMP4_5_6    = 65,
    IT_COMP7        = 66,
    /**
     * RESERVED [67-73]
     */
    IT_USB_HP       = 74,
    IT_USB_LP       = 75,
    IT_USB_WKUP     = 76,
    /**
     * RESERVED [77-80]
     */
    IT_FPU          = 81
} it_channel;

/**
 * supervisor call codes enumeration
 */
typedef enum sv_code_t {
    /**
     * switch to user mode
     */
    SVC_USER_MODE   = 0,
    /**
     * switch to kernel mode
     */
    SVC_KERNEL_MODE = 1,
    /**
     * execute in privileged mode
     */
    SVC_EXECUTE     = 2
} sv_code;

/**
 * @brief      enable interrupt by given channel
 *
 * @param[in]  channel  interrupt channel
 */
void nvic_it_enable(it_channel channel);

/**
 * @brief      disable interrupt by given channel
 *
 * @param[in]  channel  interrupt channel
 */
void nvic_it_disable(it_channel channel);

/**
 * @brief      Handler for unused interrupts
 */
void default_handler(void);

/**
 * @brief      CPU hard fault handler
 *
 * @param[in]  stack  register frame
 */
void h_fault_handler(uint32_t stack[]);

/**
 * @defgroup USART_INTERRUPTS usart/uart interrupt routines
 *
 * TODO: move it out of here
 *
 * @{
 */

void isr_usart1(void);

void isr_usart2(void);

/** @} */

/**
 * @brief      systick interrupt
 */
void systick_handler(void);
/**
 * @brief      pends execution of pend_sv_handler (used by systick handler)
 */
void pend_sv_call(void);

/**
 * @brief      pendSV exception handler. Performs context switch routine
 */
void pend_sv_handler(void);

/**
 * @brief      performs SV Call with given parameter
 *
 * @param[in]  svc_number  number of supervisor function to call
 */
void sv_call(sv_code svc_number);

/**
 * @brief      switch to user mode
 */
void sv_user_mode(void);

/**
 * @brief      switch to kernel mode
 */
void sv_kernel_mode(void);

/**
 * @brief      SV Call exception handler. Performs swith to user mode
 *
 * @param[in]  svc_args  supervisor call arguments
 */
void sv_call_handler(void);

#endif
