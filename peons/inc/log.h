#ifndef PEON_LOG_H
#define PEON_LOG_H

#include "common/common.h"
#include "kernel/memory.h"
#include "lib/string.h"

#define LOG_BUF_SIZE 100

/**
 * @brief      log entry header
 */
typedef struct log_entry_t {
    struct log_entry_t *ptr;
    char *message;
} log_entry;

void log_task(void);

/**
 * @brief      add new log
 *
 * @param[in]  message  the log message
 */
void log_add(const char* message, ...);

/**
 * @brief      clears list of logs
 */
void log_clear(void);

/**
 * @brief      get new log
 *
 * @return     log message
 */
char* log_get(void);

/**
 * @brief      should be executed before reading logs
 */
void log_start_read(void);

/**
 * @brief      outputs system logs
 */
void log_output(void);

#endif
