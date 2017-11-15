#ifndef GPIO_H
#define GPIO_H

#include "arch.h"

/**
 * adresses of GPIO register sets
 */
typedef enum {
    GPIO_A = GPIOA_BASE,
    GPIO_B = GPIOB_BASE,
    GPIO_C = GPIOC_BASE,
    GPIO_D = GPIOD_BASE,
    GPIO_E = GPIOE_BASE,
    GPIO_F = GPIOF_BASE,
    GPIO_G = GPIOG_BASE
} gpio_port;

/**
 * pin mode
 */
typedef enum {
    GPIO_INPUT  = 0x0,
    GPIO_OUTPUT = 0x1,
    GPIO_AF     = 0x2,
    GPIO_ANALOG = 0x3
} gpio_mode;

/**
 * pin output type
 */
typedef enum {
    GPIO_PUSH_PULL  = 0x0,
    GPIO_OPEN_DRAIN = 0x1
} gpio_otype;

/**
 * pin transmission speed
 */
typedef enum {
    GPIO_LOW_SPEED    = 0x0,
    GPIO_MEDIUM_SPEED = 0x1,
    GPIO_HIGH_SPEED   = 0x2,
    GPIO_FAST_SPEED   = 0x3
} gpio_speed;

/**
 * pin pull-up/pull-down mode
 */
typedef enum {
    GPIO_NO_PUPD   = 0x0,
    GPIO_PULL_UP   = 0x1,
    GPIO_PULL_DOWN = 0x2
} gpio_pupd;

/**
 * pin alternate functions
 */
typedef enum {
    GPIO_AF0  = 0x0,
    GPIO_AF1  = 0x1,
    GPIO_AF2  = 0x2,
    GPIO_AF3  = 0x3,
    GPIO_AF4  = 0x4,
    GPIO_AF5  = 0x5,
    GPIO_AF6  = 0x6,
    GPIO_AF7  = 0x7,
    GPIO_AF8  = 0x8,
    GPIO_AF9  = 0x9,
    GPIO_AF10 = 0xa,
    GPIO_AF11 = 0xb,
    GPIO_AF12 = 0xc,
    GPIO_AF13 = 0xd,
    GPIO_AF14 = 0xe,
    GPIO_AF15 = 0xf
} gpio_af;

/**
 * @defgroup GPIO_REGISTERS get address of GPIO register
 * by base GPIO address
 *
 * @{
 */

volatile uint32_t *gpio_moder(volatile uint32_t *base_addr);
volatile uint32_t *gpio_otyper(volatile uint32_t *base_addr);
volatile uint32_t *gpio_ospeedr(volatile uint32_t *base_addr);
volatile uint32_t *gpio_pupdr(volatile uint32_t *base_addr);
volatile uint32_t *gpio_idr(volatile uint32_t *base_addr);
volatile uint32_t *gpio_odr(volatile uint32_t *base_addr);
volatile uint32_t *gpio_bsrr(volatile uint32_t *base_addr);
volatile uint32_t *gpio_lckr(volatile uint32_t *base_addr);
volatile uint32_t *gpio_afrl(volatile uint32_t *base_addr);
volatile uint32_t *gpio_afrh(volatile uint32_t *base_addr);
volatile uint32_t *gpio_brr(volatile uint32_t *base_addr);

/** @} */

/**
 * @brief      Initializes GPIO pin
 *
 * @param[in]  port   GPIO port
 * @param[in]  pin    pin number
 * @param[in]  otype  output type
 * @param[in]  mode   outup mode
 * @param[in]  speed  output speed
 * @param[in]  pupd   pull-up/pull-down
 * @param[in]  af     alternate function(ignored if mode is not AF)
 */
void gpio_init_pin(gpio_port port, uint32_t pin, gpio_otype otype,
    gpio_mode mode, gpio_speed speed, gpio_pupd pupd, gpio_af af);
/**
 * @brief      set pin output value to 0
 *
 * @param[in]  port   GPIO port
 * @param[in]  pin    GPIO pin
 */
void gpio_low(gpio_port port, uint32_t pin);
/**
 * @brief      set pin output value to 1
 *
 * @param[in]  port   GPIO port
 * @param[in]  pin    GPIO pin
 */
void gpio_high(gpio_port port, uint32_t pin);
/**
 * @brief      switch pin mode to input
 *
 * @param[in]  port   GPIO port
 * @param[in]  pin    GPIO pin
 */
void gpio_input(gpio_port port, uint32_t pin);
/**
 * @brief      switch pin mode to output
 *
 * @param[in]  port   GPIO port
 * @param[in]  pin    GPIO pin
 */
void gpio_output(gpio_port port, uint32_t pin);
/**
 * @brief      read input value of pin
 *
 * @param[in]  port   GPIO port
 * @param[in]  pin    GPIO pin
 *
 * @return     pin input value
 * @retval     false  low voltage
 * @retval     true   high voltage
 */
bool gpio_read(gpio_port port, uint32_t pin);

#endif
