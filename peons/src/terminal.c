#include "peons/terminal.h"
#include "utils/lcd.h"
#include "utils/pony.h"
#include "utils/delay.h"
#include "utils/echo.h"
#include "utils/putil.h"
#include "utils/cutil.h"
#include "utils/lutil.h"
#include "utils/sensor.h"
#include "utils/help.h"
#include "board/cfg.h"

/**
 * terminal availability flag
 */
static bool terminal_busy = false;

/**
 * @brief      array of command contexts
 */
static const terminal_command_context terminal_command_list[] = {
    {
        .terminal_cmd_handler    = pony_cmd_handler,
        .terminal_command_string = COMMAND_STR_PONY
    }, {
        .terminal_cmd_handler    = delay_cmd_handler,
        .terminal_command_string = COMMAND_STR_DELAY
    }, {
        .terminal_cmd_handler    = echo_cmd_handler,
        .terminal_command_string = COMMAND_STR_ECHO
    }, {
        .terminal_cmd_handler    = cutil_cmd_handler,
        .terminal_command_string = COMMAND_STR_CUTIL
    }, {
        .terminal_cmd_handler    = sensor_cmd_handler,
        .terminal_command_string = COMMAND_STR_SENSOR
    }, {
        .terminal_cmd_handler    = lutil_cmd_handler,
        .terminal_command_string = COMMAND_STR_LUTIL
    }, {
        .terminal_cmd_handler    = help_cmd_handler,
        .terminal_command_string = COMMAND_STR_HELP
    }, {
        .terminal_cmd_handler    = putil_cmd_handler,
        .terminal_command_string = COMMAND_STR_PUTIL
    }, {
        .terminal_cmd_handler    = lcd_cmd_handler,
        .terminal_command_string = COMMAND_STR_LCD
    }
};

/**
 * buffer for storing terminal command
 */
static char terminal_input_buffer[TERMINAL_INPUT_BUFFER_SIZE];

/**
 * terminal buffer character counter
 */
static uint32_t input_buffer_counter = 0;

/**
 * @brief      clears terminal command buffer
 */
static void terminal_clear_input_buffer(void) {
    for (int i = 0; i < TERMINAL_INPUT_BUFFER_SIZE; ++i) {
        terminal_input_buffer[i] = '\0';
    }
}

/**
 * @brief      get argument from terminal input buffer delimited by spaces
 *
 * NOTE:       treats wrapped in quoutes as one argument, ignoring spaces
 *
 * @param      buffer     input buffer
 * @param[in]  start_pos  start position to seek argument
 *
 * @return     allocated string with argument
 */
static char *terminal_get_arg(char *buffer, uint32_t start_pos) {
    char *begin = &buffer[start_pos];
    char *end = begin;

    //! look for argument begin
    while ((*begin == ' ' || *begin == '\"') && *begin) {
        begin++;
        if (*(begin - 1) == '\"') {
            break;
        }
    }

    //! no more arguments left
    if (*begin == 0) {
        return NULL;
    }

    end = begin;

    //! looking for the end of wrapped argument
    if (*(begin - 1) == '\"') {
        while (*end != '\"' && *end) {
            end++;
        }

        if (*end == 0) {
            return NULL;
        }
    } else {
        while (*end != ' ' && *end) {
            end++;
        }
    }


    end--;
    int str_len = end - begin + 1;

    //! allocating and copying argument string
    char *arg = cell_alloc(str_len + 1);
    memcpy(arg, begin, str_len);
    arg[str_len] = 0;

    return arg;
}

/**
 * @brief      create list of arguments based on input buffer
 *
 * @param      buffer  input buffer
 *
 * @return     arguments list interface
 */
static list_iface *terminal_parse_input(char* buffer) {
    list_iface *command = list_create();
    if (!command) {
        return NULL;
    }

    int token = 0;

    char *arg = terminal_get_arg(buffer, token);
    while (arg) {
        if (!command->add(command, arg, strsize(arg))) {
            command->destroy(command);
            cell_free(arg);
            return NULL;
        }
        token += strsize(arg);
        cell_free(arg);
        arg = terminal_get_arg(buffer, token);
    }

    cell_free(arg);
    return command;
}

/**
 * @brief      command-line parser/executor
 *
 * @return     succes/failure of execution
 */
static bool terminal_process_command(void) {
    //! error flag
    bool ret = false;
    //! create arguments list
    list_iface *command = terminal_parse_input(terminal_input_buffer);
    if (!command) {
        return ret;
    }

    //! look for received command in commands list
    for (unsigned int i = 0; i < sizeof(terminal_command_list)/
        sizeof(terminal_command_list[0]); ++i) {
        if (strcmp(command->get(command, 0), terminal_command_list[i].
            terminal_command_string) == 0) {

            ret = (terminal_command_list[i].terminal_cmd_handler(command) == 0);

            goto end;
        }
    }

    end :
    //! free memory allocated for argument list
    command->destroy(command);
    return ret;
}

void terminal_init(void) {
    //! initialize STDIO
    init_usart(STDIO, DEFAULT_BAUD_RATE);
}

void terminal_new_cmd(void) {
    terminal_clear_input_buffer();
    input_buffer_counter = 0;
    put_string(STDIO, "> ");
}

bool terminal_available(void) {
    return !terminal_busy;
}

void terminal_start(void) {
    terminal_new_cmd();
    while (true) {
        if (receiver_available(STDIO)) {
            terminal_busy = true;
            terminal_input_buffer[input_buffer_counter] = get_char(STDIO);
            if (terminal_input_buffer[input_buffer_counter] != BACKSPACE) {
                put_char(STDIO, terminal_input_buffer[input_buffer_counter]);
            } else if (input_buffer_counter > 0) {
                put_char(STDIO, terminal_input_buffer[input_buffer_counter]);
                terminal_input_buffer[input_buffer_counter] = '\0';
                input_buffer_counter--;
                continue;
            } else {
                terminal_clear_input_buffer();
                input_buffer_counter = 0;
                continue;
            }

            if (input_buffer_counter >= TERMINAL_INPUT_BUFFER_SIZE) {
                put_newline(STDIO);
                error_message("Input bufer overflow.");
                terminal_new_cmd();
            } else if (terminal_input_buffer[input_buffer_counter] == NEWLINE) {
                terminal_input_buffer[input_buffer_counter] = 0;
                if (input_buffer_counter > 0 && !terminal_process_command()) {
                    error_message("Invalid command/arguments. "
                        "Type \"help\" for list of available commands.");
                }
                terminal_new_cmd();
                terminal_busy = false;
            } else {
                input_buffer_counter++;
            }
        }
    }
}
