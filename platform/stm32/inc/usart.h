#ifndef USART_H
#define USART_H

#include "common/common.h"

#define USART_BUF_SIZE 512

#define USART_1 1
#define USART_2 2
#define USART_3 3
#define UART_4  4
#define UART_5  5

#define NEWLINE  0x0d
#define LINEFEED 0x0a
#define BACKSPACE 0x08

/**
 * @brief      write char to usart/uart software buffer
 *
 * @param[in]  usart_number  usart/uart number
 * @param[in]  data          data byte to be written
 */
void usart_write_buf(const uint32_t usart_number, const char data);

/**
 * @brief      get usart/uart software buffer pointer
 *
 * @param[in]  usart_number  usart/uart number
 *
 * @return     buffer pointer
 */
char *get_usart_buf(const uint32_t usart_number);

/**
 * @brief      clears usart/uart software buffer
 *
 * @param[in]  usart_number  usart/uart number
 */
void usart_clear_buf(const uint32_t usart_number);

/**
 * @brief      RX/TX availability indicators
 *
 * @param[in]  usart_number  The usart/uart number
 *
 * @return     RX/TX availability
 */
bool transmitter_available(const uint32_t usart_number);
bool receiver_available(const uint32_t usart_number);

/**
 * @brief      Puts a character to the desired usart/uart
 *
 * @param[in]  usart_number  The usart/uart number
 * @param[in]  data          The 8bit data
 */
void put_char(const uint32_t usart_number, const char data);
void put_char_unsafe(const uint32_t usart_number, const char data);

/**
 * @brief      Gets the character from the desired usart/uart
 *
 * @param[in]  usart_number  The usart number
 *
 * @return     The 8bit data
 */
char get_char(const uint32_t usart_number);
char get_char_unsafe(const uint32_t usart_number);

/**
 * @brief      Initializes the desired usart/uart
 *
 * @param[in]  usart_number  The usart/uart number
 * @param[in]  baud_rate     The baud rate
 */
void init_usart(const uint32_t usart_number, const uint32_t baud_rate);

/**
 * @brief      Puts a string/line/newline to the desired usart/uart
 *
 * @param[in]  usart_number  The usart/uart number
 * @param[in]  data          The string pointer
 * @param[in]  len           The string length
 */
void nput_string(const uint32_t usart_number, const char *data, uint32_t len);
void put_string(const uint32_t usart_number, const char *data);
void put_line(const uint32_t usart_number, const char *data);
void put_newline(const uint32_t usart_number);

#endif
