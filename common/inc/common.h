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

typedef enum {
    false,
    true
} bool;

#endif