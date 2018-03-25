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

static const usart_config stdio_config = {
    .baud_rate   = DEFAULT_BAUD_RATE,
    .buffer_size = TERMINAL_INPUT_BUFFER_SIZE,
    .port        = USART2_PORT,
    .tx_pin      = USART2_TX_PIN,
    .rx_pin      = USART2_RX_PIN
};

static usart_iface *stdio = NULL;

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
 * @brief      get argument from terminal input buffer delimited by spaces
 *             or get ptr to the end of last argument found
 *
 * NOTE:       treats wrapped in quoutes as one argument, ignoring spaces
 *
 * @param      buffer     input buffer (NULL if you want to get ptr
 *                        to the end of last argument)
 * @param[in]  start_pos  start position to seek argument
 *
 * @return     allocated string with argument
 */
static char *terminal_get_arg(const char *buffer, char *begin) {
    static char *static_token = NULL;

    if (!buffer && static_token) {
        return static_token;
    }

    char *end   = begin;

    //! look for argument begin
    while ((*begin == ' ' || *begin == '\"') && *begin) {
        begin++;
    }

    //! no more arguments left
    if (*begin == '\0') {
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

    static_token = end + 1;

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
static list_iface *terminal_parse_input(const char* buffer) {
    list_iface *command = list_create();
    if (!command) {
        return NULL;
    }

    char *token = (char*) buffer;

    char *arg = terminal_get_arg(buffer, token);
    while (arg) {
        if (!command->add(command, arg, strsize(arg))) {
            command->destroy(command);
            cell_free(arg);
            return NULL;
        }

        token = terminal_get_arg(NULL, 0);
        if (!token) {
            break;
        }

        cell_free(arg);
        arg = terminal_get_arg(buffer, token);
    }

    cell_free(arg);
    return command;
}

/**
 * @brief      command-line parser/executor
 *
 * @param[in]  buffer  command buffer to parse
 *
 * @return     succes/failure of execution
 */
static bool terminal_process_command(const char *buffer) {
    //! error flag
    bool ret = false;
    //! create arguments list
    list_iface *command = terminal_parse_input(buffer);
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
    usart_init(STDIO, &stdio_config);
    stdio = usart_iface_get(STDIO);
}

void terminal_new_cmd(void) {
    stdio->puts(stdio, "\n> ");
}

void terminal_loop(void) {
    if (!stdio) {
        while(true);
    }
    terminal_new_cmd();
    while (true) {
        if (stdio->buffer_locked(stdio)) {
            const char *cmd_buffer = stdio->buffer_drain(stdio);
            if (!terminal_process_command(cmd_buffer)) {
                error_message("Invalid command/arguments. "
                        "Type \"help\" for list of available commands.");
            }
            terminal_new_cmd();
            stdio->set_buffer_locked(stdio, false);
        }
    }
}
