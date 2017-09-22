#ifndef TERMINAL_H
#define TERMINAL_H

#include "string.h"
#include "usart.h"
#include "clock.h"

#define TERMINAL_INPUT_BUFFER_SIZE 12

#define TERMINAL_COMMAND_NUMBER 3
#define TERMINAL_ARG_MAX_SIZE 8

#define TERMINAL_ARG_NONE 0
#define TERMINAL_ARG_INT 1
#define TERMINAL_ARG_STR 2

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
    char terminal_command_string[TERMINAL_ARG_MAX_SIZE];
    /**
     * type of function argument
     */
    uint8_t terminal_command_arg;
} terminal_command_context;

/**
 * @brief      initialization of usart/uart used by terminal i/o
 */
void terminal_init(void);

/**
 * @brief      main terminal loop
 */
void terminal_start(void);

/**
 * @brief      command-line parser/executor
 *
 * @param[in]  input_buffer_counter  current size of command
 *
 * @return     succes/failure of execution
 */
bool terminal_process_command(const uint32_t input_buffer_counter);

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
 * @brief      put pony art into terminal
 */
void terminal_draw_pony(void);

/**
 * @brief      clears terminal command buffer
 */
void terminal_clear_input_buffer(void);

#endif
