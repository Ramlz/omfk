#ifndef TERMINAL_H
#define TERMINAL_H

#include "string.h"
#include "usart.h"
#include "memory.h"
#include "sensor.h"
#include "timer.h"
#include "log.h"
#include "putil.h"
#include "list.h"

#define TERMINAL_INPUT_BUFFER_SIZE 64

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
    void (*terminal_command_function)();
    /**
     * command string to execute function
     */
    const char *terminal_command_string;
    /**
     * types of function arguments
     */
    const char *terminal_command_args;
} terminal_command_context;

/**
 * @brief      prepare terminal before input
 */
void terminal_new_cmd(void);

/**
 * @brief      check if terminal can be interrupted by foreign output
 *
 * @return     terminal availability
 */
bool terminal_available(void);

/**
 * @brief      initialization of usart/uart used by terminal i/o
 */
void terminal_init(void);

/**
 * @brief      main terminal loop
 */
void terminal_start(void);

/**
 * @brief      puts debug messge into terminal
 *
 * @param[in]  message  string with message
 * @param[in]  error    error/info message flag
 */
void terminal_message(const char *message, const bool error);

/**
 * @brief      wrapper for error terminal message
 *
 * @param[in]  message  string with message
 */
void terminal_error_message(const char *message);

/**
 * @brief      wrapper for info terminal message
 *
 * @param[in]  message  string with message
 */
void terminal_info_message(const char *message);

/**
 * @brief      put ASCII pony art to terminal output or bitmap image to lcd
 *
 * @param      output_destination  destination of output
 */
void terminal_draw_pony(char *output_destination);

/**
 * @brief      print formatted data to STDIO
 *
 * @param[in]  fmt        formatting string
 */
void terminal_printf(const char *fmt, ...);

/**
 * @brief      outputs system logs
 */
void terminal_output_logs(void);

/**
 * @brief      output terminal help
 */
void terminal_help(void);

#endif
