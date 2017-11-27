#ifndef LOG_H
#define LOG_H

#include "common.h"
#include "memory.h"
#include "string.h"

#define LOG_MAX 16

/**
 * @brief      log entry header
 */
typedef struct log_entry_t {
    struct log_entry_t *ptr;
    char *message;
} log_entry;

/**
 * @brief      add new log
 *
 * @param[in]  message  the log message
 */
void log_add(const char* message);

/**
 * @brief      clears list of logs
 */
void log_clear(void);

/**
 * @brief      delete log list node
 *
 * @param      entry  the log entry
 */
static void log_delete_entry(log_entry *entry);

/**
 * @brief      get new log
 *
 * @return     log message
 */
char* log_get(void);

#endif
