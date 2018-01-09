#include "usart.h"

static char usart1_buf[USART_BUF_SIZE] = "";

static uint32_t usart1_cnt = 0;

void usart_write_buf(const uint32_t usart_number, const char data) {
    switch(usart_number) {
        case USART_1:
            usart1_buf[usart1_cnt++] = data;
            break;
        case USART_2:
            break;
        case USART_3:
            break;
        case UART_4:
            break;
        case UART_5:
            break;
        default:
            break;
    }
}

char *get_usart_buf(const uint32_t usart_number) {
    switch(usart_number) {
        case USART_1:
            return usart1_buf;
        case USART_2:
            break;
        case USART_3:
            break;
        case UART_4:
            break;
        case UART_5:
            break;
        default:
            break;
    }
    return NULL;
}

void usart_clear_buf(const uint32_t usart_number) {
    switch(usart_number) {
        case USART_1:
            usart1_cnt = 0;
            break;
        case USART_2:
            break;
        case USART_3:
            break;
        case UART_4:
            break;
        case UART_5:
            break;
        default:
            break;
    }
}

bool transmitter_available(const uint32_t usart_number) {
    switch(usart_number) {
        case USART_1:
            return USART1_ISR & BIT7;
            break;
        case USART_2:
            return USART2_ISR & BIT7;
        case USART_3:
            return USART3_ISR & BIT7;
        case UART_4:
            break;
        case UART_5:
            break;
        default:
            break;
    }
    return false;
}

bool receiver_available(const uint32_t usart_number) {
    switch(usart_number) {
        case USART_1:
            return USART1_ISR & BIT5;
        case USART_2:
            return USART2_ISR & BIT5;
        case USART_3:
            return USART3_ISR & BIT5;
        case UART_4:
            break;
        case UART_5:
            break;
        default:
            break;
    }
    return false;
}

void put_char(const uint32_t usart_number, const char data) {
    while (!transmitter_available(usart_number));
    if (data == NEWLINE) {
        put_char_unsafe(usart_number, LINEFEED);
    } else if (data == BACKSPACE) {
        put_char_unsafe(usart_number, data);
        while (!transmitter_available(usart_number));
        put_char_unsafe(usart_number, ' ');
    }
    while (!transmitter_available(usart_number));
    put_char_unsafe(usart_number, data);
}

void put_char_unsafe(const uint32_t usart_number, const char data) {
    switch(usart_number) {
        case USART_1:
            USART1_TDR = data;
            break;
        case USART_2:
            USART2_TDR = data;
            break;
        case USART_3:
            USART3_TDR = data;
            break;
        case UART_4:
            break;
        case UART_5:
            break;
        default: 
            break;
    }
}

char get_char(const uint32_t usart_number) {
    while (!receiver_available(usart_number));
    return get_char_unsafe(usart_number);
}

char get_char_unsafe(const uint32_t usart_number) {
    switch(usart_number) {
        case USART_1:
            return USART1_RDR;
            break;
        case USART_2:
            return USART2_RDR;
            break;
        case USART_3:
            return USART3_RDR;
        case UART_4:
            break;
        case UART_5:
            break;
        default: 
            break;
    }
    return NULL;
}

void init_usart(const uint32_t usart_number, const uint32_t baud_rate) {
    const uint32_t baud_rate_divisor = 32000000 / baud_rate;

    disable_interrupts();

    switch(usart_number) {
        case USART_1:
            // init Tx pin 
            gpio_init_pin(GPIO_A, 9, GPIO_PUSH_PULL, GPIO_AF, GPIO_LOW_SPEED,
                GPIO_NO_PUPD, GPIO_AF7);
            // init Rx pin
            gpio_init_pin(GPIO_A, 10, GPIO_PUSH_PULL, GPIO_AF, GPIO_LOW_SPEED,
                GPIO_NO_PUPD, GPIO_AF7);
            // enable usart1 clock
            RCC_APB2ENR |= BIT14;

            USART1_BRR = baud_rate_divisor;
            USART1_CR1 |= (BIT2 | BIT3 | BIT5);
            USART1_CR1 |= BIT0;

            ISER1 = (1 << 5);
            break;
        case USART_2:
            // init Tx pin 
            gpio_init_pin(GPIO_A, 2, GPIO_PUSH_PULL, GPIO_AF, GPIO_LOW_SPEED,
                GPIO_NO_PUPD, GPIO_AF7);
            // init Rx pin
            gpio_init_pin(GPIO_A, 3, GPIO_PUSH_PULL, GPIO_AF, GPIO_LOW_SPEED,
                GPIO_NO_PUPD, GPIO_AF7);
            // enable usart2 clock
            RCC_APB1ENR |= BIT17;

            USART2_BRR = baud_rate_divisor;
            USART2_CR1 |= (BIT2 | BIT3 | BIT5 | BIT7); 
            USART2_CR1 |= BIT0;
            break;
        case USART_3:
            break;
        case UART_4:
            break;
        case UART_5:
            break;
    }

    enable_interrupts();
}

void put_string(const uint32_t usart_number, const char *data) {
    while (*data) {
        put_char(usart_number, *data++);
    }
}

void nput_string(const uint32_t usart_number, const char *data, uint32_t len) {
    while (*data && len--) {
        put_char(usart_number, *data++);
    }
}

void put_line(const uint32_t usart_number, const char *data) {
    put_string(usart_number, data);
    put_newline(usart_number);
}

void put_newline(const uint32_t usart_number) {
    put_char(usart_number, NEWLINE);
}
