#ifndef GPIO_H
#define GPIO_H

#include "peripheral.h"

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
 * GPIO port interfaces
 */
typedef struct gpio_iface_t {
    /**
     * @brief      Initializes GPIO pin
     *
     * @param[in]  iface  GPIO interface
     * @param[in]  pin    pin number
     * @param[in]  otype  output type
     * @param[in]  mode   outup mode
     * @param[in]  speed  output speed
     * @param[in]  pupd   pull-up/pull-down
     * @param[in]  af     alternate function(ignored if mode is not AF)
     */
    void (*init)(struct gpio_iface_t *iface, uint32_t pin, gpio_otype otype,
        gpio_mode mode, gpio_speed speed, gpio_pupd pupd, gpio_af af);

    /**
     * @brief      read input value of pin
     *
     * @param[in]  iface  GPIO interface
     * @param[in]  pin    GPIO pin
     *
     * @return     pin input value
     * @retval     false  low voltage
     * @retval     true   high voltage
     */
    bool (*read)(struct gpio_iface_t *iface, uint32_t pin);

    /**
     * @brief      write output value of GPIO pin
     *
     * @param      iface  GPIO interface
     * @param[in]  pin    GPIO pin
     * @param      value  pin output value
     *
     * @return     { description_of_the_return_value }
     */
    void (*write)(struct gpio_iface_t *iface, uint32_t pin, bool value);

    /**
     * @brief      toggle pin output value
     *
     * @param[in]  iface  GPIO interface
     * @param[in]  pin    GPIO pin
     */
    void (*toggle)(struct gpio_iface_t *iface, uint32_t pin);

    /**
     * @brief      switch pin mode
     *
     * @param[in]  iface  GPIO interface
     * @param[in]  pin    GPIO pin
     * @param[in]  mode   GPIO mode
     */
    void (*mode)(struct gpio_iface_t *iface, uint32_t pin, gpio_mode mode);
} gpio_iface;

/**
 * @brief      create GPIO context and initialize it
 *
 * @param      port  GPIO port address
 *
 * @return     allocated interface ptr
 */
gpio_iface *gpio_iface_create(gpio_port port);

/**
 * @brief      deallocate GPIO context
 *
 * @param      iface  GPIO interface
 */
void gpio_iface_destroy(gpio_iface *iface);

#endif
