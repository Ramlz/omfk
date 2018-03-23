#ifndef CFG_H
#define CFG_H

#define HAS_GPIO_A
#define HAS_GPIO_B
#define HAS_GPIO_C
#define HAS_GPIO_D

#define HAS_TIM1
#define HAS_TIM2
#define HAS_TIM3
#define HAS_TIM4
#define HAS_TIM6
#define HAS_TIM7
#define HAS_TIM8
#define HAS_TIM15
#define HAS_TIM16
#define HAS_TIM17
#define HAS_TIM20

#define USART1_PORT GPIOA
#define USART1_TX_PIN 9
#define USART1_RX_PIN 10

#define USART2_PORT GPIOA
#define USART2_TX_PIN 2
#define USART2_RX_PIN 15

#define LED_GPIO_PORT GPIOB
#define LED_GPIO_PIN 3

#define DHT_GPIO_PORT GPIOC
#define DHT_GPIO_PIN  7
#define DHT_TIMER     TIM1

#define STDIO USART_2

#define DEFAULT_BAUD_RATE 38400

#define PCD8544_PORT    GPIOB
#define PCD8544_RST_PIN 11
#define PCD8544_PIN_CE  12
#define PCD8544_PIN_DC  13
#define PCD8544_PIN_DIN 14
#define PCD8544_PIN_CLK 15

#endif
