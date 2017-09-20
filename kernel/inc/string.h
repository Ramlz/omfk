#ifndef STRING_H
#define STRING_H

#define STR_BUF_SIZE 24

#include <stdarg.h>
/**
 * @brief      compare two strings
 *
 * @param[in]  str1  the string 1
 * @param[in]  str2  the string 2
 * @param[in]  n     bytes to compare
 *
 * @return     result of comparison
 */
int strncmp(const char *str1, const char *str2, int n);
/**
 * @brief      get length of string
 *
 * @param[in]  str   The string
 *
 * @return     lenght of the string
 */
int strlen(const char *str);
/**
 * @brief      implementation of functions from prinf family
 *
 * @param      buffer      buffer to store result
 * @param[in]  buffer_len  the buffer length
 * @param[in]  fmt         formatting string
 * @param[in]  va          arguments to put
 *
 * @return     number of characters put into buffer
 */
int snprintf(char *buffer, unsigned int buffer_len, const char *fmt, ...);
int vsnprintf(char *buffer, unsigned int buffer_len,
    const char *fmt, va_list va);
/**
 * @brief      implementation of functions from scanf family
 *
 * @param[in]  str        the string to parse
 * @param[in]  fmt        formatting string
 * @param[in]  va         arguments to take
 *
 * @return     number of items in the argument list successfully filled
 */
int sscanf(const char* str, const char* fmt, ...);
int vsscanf(const char* str, const char* fmt, va_list va);
/**
 * @brief      contert integer to string
 *
 * @param[in]  value      value to be converted to a string
 * @param[in]  radix      the integer radix
 * @param[in]  uppercase  use/dont use uppercase flag
 * @param[in]  unsig      signed/unsigned value flag
 * @param      buffer     the buffer to store result
 * @param[in]  zero_pad   desired number of digits (filling extra with zero)
 *
 * @return     length of result string
 */
static unsigned int itoa(int value, unsigned int radix, unsigned int uppercase,
    unsigned int unsig, char *buffer, unsigned int zero_pad);

#endif
