#include "gpio.h"

volatile uint32_t *gpio_moder(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 0);
}

volatile uint32_t *gpio_otyper(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 1);
}

volatile uint32_t *gpio_ospeedr(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 2);
}

volatile uint32_t *gpio_pupdr(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 3);
}

volatile uint32_t *gpio_idr(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 4);
}

volatile uint32_t *gpio_odr(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 5);
}

volatile uint32_t *gpio_bsrr(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 6);
}

volatile uint32_t *gpio_lckr(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 7);
}

volatile uint32_t *gpio_afrl(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 8);
}

volatile uint32_t *gpio_afrh(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 9);
}

volatile uint32_t *gpio_brr(volatile uint32_t *base_addr) {
    return (volatile uint32_t*) (base_addr + 10);
}

void gpio_init_pin(gpio_port port, uint32_t pin, gpio_otype otype,
    gpio_mode mode, gpio_speed speed, gpio_pupd pupd, gpio_af af) {
    volatile uint32_t *port_addr = (volatile uint32_t*) port;
    switch (port) {
        case GPIO_A:
            // turn on port clock if not yet enabled
            if (!(RCC_AHBENR & BIT17)) {
                RCC_AHBENR   |= BIT17;
            }
            break;
        case GPIO_B:
            // turn on port clock if not yet enabled
            if (!(RCC_AHBENR & BIT18)) {
                RCC_AHBENR |= BIT18;
            }
            break;
        case GPIO_C:
            // turn on port clock if not yet enabled
            if (!(RCC_AHBENR & BIT19)) {
                RCC_AHBENR |= BIT19;
            }
            break;
        case GPIO_D:
            // turn on port clock if not yet enabled
            if (!(RCC_AHBENR & BIT20)) {
                RCC_AHBENR |= BIT20;
            }
            break;
        case GPIO_E:
            // turn on port clock if not yet enabled
            if (!(RCC_AHBENR & BIT21)) {
                RCC_AHBENR |= BIT21;
            }
            break;
        case GPIO_F:
            // turn on port clock if not yet enabled
            if (!(RCC_AHBENR & BIT22)) {
                RCC_AHBENR |= BIT22;
            }
            break;
        case GPIO_G:
            // turn on port clock if not yet enabled
            if (!(RCC_AHBENR & BIT23)) {
                RCC_AHBENR |= BIT23;
            }
            break;
        default:
            return;
    }
    //set mode register
    *gpio_moder((volatile uint32_t*) port)   |= (mode << (pin * 2));
    // set otype register
    *gpio_otyper((volatile uint32_t*) port)  |= (otype << pin);
    // set ospeed register
    *gpio_ospeedr((volatile uint32_t*) port) |= (speed << (pin * 2));
    //set pull-up/pull-down register
    *gpio_pupdr((volatile uint32_t*) port)   |= (pupd << (pin * 2));
    // set alternate function (if any)
    if (mode == GPIO_AF) {
        if (pin < 8) {
            *gpio_afrl((volatile uint32_t*) port) |= (af << (pin * 4));
        } else {
            *gpio_afrh((volatile uint32_t*) port) |= (af << ((pin - 8) * 4));
        }
    }
}

void gpio_low(gpio_port port, uint32_t pin) {
    *gpio_bsrr((volatile uint32_t*) port) |= (0x1 << (16 + pin));
}

void gpio_high(gpio_port port, uint32_t pin) {
    *gpio_bsrr((volatile uint32_t*) port) |= (0x1 << pin);
}

void gpio_input(gpio_port port, uint32_t pin) {
    //set mode register
    *gpio_moder((volatile uint32_t*) port) |= (GPIO_INPUT << (pin * 2));
}

void gpio_output(gpio_port port, uint32_t pin) {
    //set mode register
    *gpio_moder((volatile uint32_t*) port) |= (GPIO_OUTPUT << (pin * 2));
}

bool gpio_read(gpio_port port, uint32_t pin) {
    return (*gpio_idr((volatile uint32_t*) port) & (0x1 << pin));
}
