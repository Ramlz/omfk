#ifndef STRING_H
#define STRING_H

#define STR_BUF_SIZE 24

#define strsize(STR) (strlen(STR) + 1)

#include "common.h"
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
 * @param[in]  str   the string
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
 * @brief      convert integer to string
 *
 * @param[in]  value      value to be converted to a string
 * @param[in]  radix      the integer radix
 * @param[in]  uppercase  use/don't use uppercase flag
 * @param[in]  unsig      signed/unsigned value flag
 * @param      buffer     the buffer to store the result
 * @param[in]  zero_pad   desired number of digits (filling extra with zero)
 *
 * @return     length of result string
 */
unsigned int itoa(int value, unsigned int radix, unsigned int uppercase,
    unsigned int unsig, char *buffer, unsigned int zero_pad);

/**
 * @brief      convert string to int
 *
 * @param      str   the string
 *
 * @return     converted integer
 */
int atoi(const char *str);

/**
 * @brief      tokenize string
 *
 * @param      str    the string
 * @param      delim  the delimiter
 *
 * @return     substring
 */
char *strtok(char *str, const char *delim);

/**
 * @brief      get span of character set in string
 *
 * @param      str1  the string 1
 * @param      str2  the string 2
 *
 * @return     length of the initial portion of str1
 * which consists only of characters that are part of str2
 */
int strspn(const char *str1,const char *str2);

/**
 * @brief      copy string
 *
 * @param      dest  the destination
 * @param      src   the source
 *
 * @return     destination is returned
 */
char * strcpy(char *dest, const char *src);

#endif
