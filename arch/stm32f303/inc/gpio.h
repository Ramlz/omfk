#ifndef GPIO_H
#define GPIO_H

#include "stm32f303xx.h"

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
    INPTUT = 0x0,
    OUTPUT = 0x1,
    AF     = 0x2,
    ANALOG = 0x3
} gpio_mode;

/**
 * pin output type
 */
typedef enum {
    PUSH_PULL  = 0x0,
    OPEN_DRAIN = 0x1
} gpio_otype;

/**
 * pin transmission speed
 */
typedef enum {
    LOW_SPEED    = 0x0,
    MEDIUM_SPEED = 0x1,
    HIGH_SPEED   = 0x2,
    FAST_SPEED   = 0x3
} gpio_speed;

/**
 * pin pull-up/pull-down mode
 */
typedef enum {
    NO_PUPD   = 0x0,
    PULL_UP   = 0x1,
    PULL_DOWN = 0x2
} gpio_pupd;

/**
 * pin alternate functions
 */
typedef enum {
    AF0  = 0x0,
    AF1  = 0x1,
    AF2  = 0x2,
    AF3  = 0x3,
    AF4  = 0x4,
    AF5  = 0x5,
    AF6  = 0x6,
    AF7  = 0x7,
    AF8  = 0x8,
    AF9  = 0x9,
    AF10 = 0xa,
    AF11 = 0xb,
    AF12 = 0xc,
    AF13 = 0xd,
    AF14 = 0xe,
    AF15 = 0xf
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

#endif
