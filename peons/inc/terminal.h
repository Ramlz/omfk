#ifndef TERMINAL_H
#define TERMINAL_H

#include "lib/string.h"
#include "platform/usart.h"
#include "kernel/memory.h"
#include "peons/log.h"
#include "kernel/peons.h"
#include "lib/list.h"

#define TERMINAL_INPUT_BUFFER_SIZE 64

#define COMMAND_STR_PONY       "pony"
#define COMMAND_STR_DELAY      "delay"
#define COMMAND_STR_ECHO       "echo"
#define COMMAND_STR_CUTIL      "cutil"
#define COMMAND_STR_SENSOR     "sensor"
#define COMMAND_STR_LUTIL      "lutil"
#define COMMAND_STR_HELP       "help"
#define COMMAND_STR_PUTIL      "putil"
#define COMMAND_STR_LCD        "lcd"

/**
 * @brief      terminal argument type token enumeration
 */
typedef enum terminal_arg_token_t {
    TERMINAL_ARG_STR = 's',
    TERMINAL_ARG_INT = 'i'
} terminal_arg_token;

/**
 * @brief      sctuct with command-related stuff
 */
typedef const struct terminal_command_context {
    /**
     * pointer to funtion command represents
     */
    int (*terminal_cmd_handler)(list_iface *args);
    /**
     * command string to execute function
     */
    const char *terminal_command_string;
} terminal_command_context;

/**
 * @brief      prepare terminal before input
 */
void terminal_new_cmd(void);

/**
 * @brief      initialization of usart/uart used by terminal i/o
 */
void terminal_init(void);

/**
 * @brief      main terminal loop
 */
void terminal_loop(void);

#endif
