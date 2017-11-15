#ifndef COMMON_H
#define COMMON_H

#include <stdarg.h>
#include <stdint.h>

#define INT_MAX __INT_MAX__
#define INT_MIN ~(__INT_MAX__ - 1)

#define NULL 0

/**
 * LD script externals
 */
extern unsigned char  INIT_DATA_VALUES;
extern unsigned char  INIT_DATA_START;
extern unsigned char  INIT_DATA_END;
extern unsigned char  BSS_START;
extern unsigned char  BSS_END;
extern unsigned char  HEAP_START;
extern unsigned char  HEAP_END;
extern unsigned char  STACK_START;
extern unsigned char  STACK_END;

/**
 * boolean typedef
 */
typedef enum {
    false,
    true
} bool;

/**
 * @brief      initialize memory with given value
 *
 * @param      ptr    pointer to memory
 * @param[in]  value  init value
 * @param[in]  size   memory size
 */
void memset(void *ptr, const uint8_t value, uint32_t size);

#endif
