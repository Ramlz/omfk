#ifndef BOARD_CFG_H
#define BOARD_CFG_H

#define HAS_GPIO_A
#define HAS_GPIO_B
#define HAS_GPIO_C
#define HAS_GPIO_D
#define HAS_GPIO_E
#define HAS_GPIO_F
#define HAS_GPIO_G
#define HAS_GPIO_H

#define USART1_PORT GPIOA
#define USART1_TX_PIN 9
#define USART1_RX_PIN 10

#define USART2_PORT GPIOA
#define USART2_TX_PIN 2
#define USART2_RX_PIN 3

#define LED_GPIO_PORT GPIOA
#define LED_GPIO_PIN 5

#define DHT_GPIO_PORT GPIOC
#define DHT_GPIO_PIN  7

#define STDIO USART_2

#define DEFAULT_BAUD_RATE 115200

#endif
