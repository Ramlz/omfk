#ifndef TERMINAL_H
#define TERMINAL_H

#include "string.h"
#include "usart.h"
#include "memory.h"
#include "dht.h"
#include "timer.h"
#include "peons.h"
#include "log.h"
#include "esp.h"
#include "list.h"

#define TERMINAL_INPUT_BUFFER_SIZE 64

#define TERMINAL_COMMAND_NUMBER 11

#define TERMINAL_ARG_NONE 0
#define TERMINAL_ARG_INT 1
#define TERMINAL_ARG_STR 2

#define STDIO USART_2

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
    char *terminal_command_string;
    /**
     * type of function argument
     */
    uint8_t terminal_command_arg;
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

/**
 * @brief      print formatted data to STDIO
 *
 * @param[in]  fmt        formatting string
 */
void terminal_printf(const char *fmt, ...);

/**
 * @brief      print data from sensors
 */
void terminal_sensor_data(const char *sensor_name);

/**
 * @brief      outputs system logs
 */
void terminal_output_logs(void);

/**
 * @brief      clear system logs
 */
void terminal_clear_logs(void);

/**
 * @brief      output terminal help
 */
void terminal_help(void);

#endif
