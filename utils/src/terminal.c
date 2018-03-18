#include "terminal.h"
#include "stat.h"
#include "context.h"
#include "board_cfg.h"

/**
 * just a string with pony...nuff said
 */
static const char pony[] =
"                                     __         __----__\r"
"                                    /  \\__..--'' `-__-__''-_\r"
"                                   ( /  \\    ``--,,  `-.''''`\r"
"                                   | |   `-..__  . \\    `.\r"
"                                   ( '. \\ _____`\\ )`-_    `.\r"
"                                   '\\  __/   __\\ ' / `:-.._\\\r"
"                                   .|\\_ (   / .-|  |-.|    ``'\r"
"                                   |' (  \\ ( (WW|  \\W)j\r"
"      ..---'''':----,             .||  ', \\_\\_`_|   ``-.\r"
"    ,'      .'` .'``-`,___        |''.   `.        \\__/\r"
"   /   _  .'  :'   _ `|.`.`-.     |   \\ ._:7,______.-'\r"
"  | .-'/  : .'  .-' `. \\`.`. `-.__':   `-\\    /\r"
"  '`  /  :' : .: .-''>-'\\ `.`-.(`-| '.    |  (\r"
"     .' :' : /   / _( \\  `.(   `.  `;.  \\  \\\r"
"     |  | .' : /|  | (____`. (     |\\   )\\  ;  |\r"
"    .' .' | | | `. |   \\  `.___.-' ) /  )/   |\r"
"    |  |  | | |  | |   ///           |/   '    |\r"
"   .' .'  '.'.`; |/ \\  /     /             \\__/\r"
"   |  |    | | |.|   |      /-,_______\\       \\\r"
"  /  / )   | | '|' _/      /     |    |\\       \\\r"
".:.-' .'  .' |   )/       /     |     | `--,    \\\r"
"     /    |  |  / |      |      |     |   /      )\r"
".__.'    /`  :|/_/|      |      |      | (       |\r"
"`-.___.-`;  / '   |      |      |      |  \\      |\r"
"       .:_-'      |       \\     |       \\  `.___/\r"
"                   \\_______)     \\_______)\r";

/**
 * terminal availability flag
 */
static bool terminal_busy = false;



/**
 * terminal command names
 */
static const char command_pony[]       = "pony";
static const char command_dly[]        = "dly";
static const char command_msg[]        = "msg";
static const char command_hstat[]      = "hstat";
static const char command_sensor[]     = "sensor";
static const char command_pstat[]      = "pstat";
static const char command_ladd[]       = "ladd";
static const char command_help[]       = "help";
static const char command_pst[]        = "pst";
static const char command_prs[]        = "prs";

/**
 * terminal command names
 */
static const char args_pony[]          = "";
static const char args_dly[]           = "i";
static const char args_msg[]           = "s";
static const char args_hstat[]         = "";
static const char args_sensor[]        = "s";
static const char args_pstat[]         = "";
static const char args_ladd[]          = "s";
static const char args_help[]          = "";
static const char args_pst[]           = "s";
static const char args_prs[]           = "s";

/**
 * @brief      array of command contexts
 */
static const terminal_command_context terminal_command_list[] = {
    {
        .terminal_command_function = TASK_PTR(terminal_draw_pony),
        .terminal_command_string   = command_pony,
        .terminal_command_args     = args_pony
    }, {
        .terminal_command_function = TASK_PTR(timer_tim1_dly_sec),
        .terminal_command_string   = command_dly,
        .terminal_command_args     = args_dly
    }, {
        .terminal_command_function = TASK_PTR(terminal_info_message),
        .terminal_command_string   = command_msg,
        .terminal_command_args     = args_msg
    }, {
        .terminal_command_function = TASK_PTR(heap_stat),
        .terminal_command_string   = command_hstat,
        .terminal_command_args     = args_hstat
    }, {
        .terminal_command_function = TASK_PTR(sensor_data),
        .terminal_command_string   = command_sensor,
        .terminal_command_args     = args_sensor
    }, {
        .terminal_command_function = TASK_PTR(peon_stat),
        .terminal_command_string   = command_pstat,
        .terminal_command_args     = args_pstat
    }, {
        .terminal_command_function = TASK_PTR(log_add),
        .terminal_command_string   = command_ladd,
        .terminal_command_args     = args_ladd
    }, {
        .terminal_command_function = TASK_PTR(terminal_help),
        .terminal_command_string   = command_help,
        .terminal_command_args     = args_help
    }, {
        .terminal_command_function = TASK_PTR(peon_stop_by_name),
        .terminal_command_string   = command_pst,
        .terminal_command_args     = args_pst
    }, {
        .terminal_command_function = TASK_PTR(peon_resume_by_name),
        .terminal_command_string   = command_prs,
        .terminal_command_args     = args_prs
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
    bool retval = false;
    //! create arguments list
    list_iface *command = terminal_parse_input(terminal_input_buffer);
    if (!command) {
        goto end;
    }

    //! look for received command in commands list
    for (unsigned int i = 0; i < sizeof(terminal_command_list)/
        sizeof(terminal_command_list[0]); ++i) {
        if (!strcmp(command->get(command, 0), terminal_command_list[i].
            terminal_command_string)) {

            int args_number = strlen(terminal_command_list[i].
                terminal_command_args);

            if (args_number + 1 != command->size(command)) {
                goto end;
            }

            void *args[args_number];

            int tmp_int = 0;

            //! transforming arguments to appropriate types
            for (int j = 0; j < args_number; ++j) {
                switch(terminal_command_list[i].
                    terminal_command_args[j]) {
                    case TERMINAL_ARG_STR:
                        args[j] = command->get(command, j + 1);
                        if (!args[j]) {
                            goto end;
                        }
                        break;
                    case TERMINAL_ARG_INT:
                        tmp_int = atoi(command->get(command, j + 1));
                        //! dirty hack, don't blame me
                        args[j] = (void*) tmp_int;
                        if (!args[j]) {
                            goto end;
                        }
                        break;
                    default:
                        goto end;
                        break;
                }
            }

            FA_START();
            //! passing command handler's arguments
            for (int j = 0; j < args_number; ++j) {
                FA_HANDLE(j, args[j]);
            }
            terminal_command_list[i].terminal_command_function();
            retval = true;
            goto end;
        }
    }
    end :
    //! free memory allocated for argument list
    command->destroy(command);
    return retval;
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
                terminal_error_message("Input bufer overflow.");
                terminal_new_cmd();
            } else if (terminal_input_buffer[input_buffer_counter] == NEWLINE) {
                terminal_input_buffer[input_buffer_counter] = 0;
                if (input_buffer_counter > 0 && !terminal_process_command()) {
                    terminal_error_message("Invalid command/arguments. "
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

void terminal_info_message(const char *message) {
    terminal_message(message, false);
}

void terminal_error_message(const char *message) {
    terminal_message(message, true);
}

void terminal_message(const char *message, const bool error) {
    //! check if it's an error message
    if (error) {
        put_string(STDIO, "[AUHTUNG] ");
    }
    put_line(STDIO, message);
}

void terminal_draw_pony(void) {
    put_string(STDIO, pony);
}

void terminal_printf(const char *fmt, ...) {
    va_list va;
    va_start(va, fmt);

    vfprintf(STDIO, fmt, va);

    va_end(va);
}

void terminal_output_logs(void) {
    peon_lock();
    {
        log_start_read();
        char *message = log_get();
        while (message) {
            terminal_info_message(message);
            message = log_get();
        }
    }
    peon_unlock();
}

void terminal_help(void) {
    terminal_printf("Available commands:\r"
                    "%s :\r"
                    "draw pony\r\r"
                    "%s [seconds] :\r"
                    "wait seconds\r\r"
                    "%s [string] :\r"
                    "output message\r\r"
                    "%s :\r"
                    "heap memory statistics\r\r"
                    "%s :\r"
                    "thread statistics\r\r"
                    "%s [log message] :\r"
                    "add system log\r\r"
                    "%s [sensor name] :\r"
                    "output sensor data\r\r"
                    "%s :\r"
                    "stop thread by it's name\r\r"
                    "%s :\r"
                    "resume thread by it's name\r\r"
                    "%s :\r"
                    "output this message\r",
                    command_pony,
                    command_dly,
                    command_msg,
                    command_hstat,
                    command_pstat,
                    command_ladd,
                    command_sensor,
                    command_pst,
                    command_prs,
                    command_help);
}
