#include "terminal.h"

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
char command_pony[]   = "pony";
char command_dly[]    = "dly";
char command_msg[]    = "msg";
char command_hstat[]  = "hstat";
char command_sensor[] = "sensor";
char command_pstat[]  = "pstat";
char command_lout[]   = "lout";
char command_lclr[]   = "lclr";
char command_ladd[]   = "ladd";
char command_espcmd[] = "espcmd";
char command_help[]   = "help";

/**
 * sensor names
 */
char sensor_dht[]     = "dht";

/**
 * @brief      array of command contexts
 */
terminal_command_context terminal_command_list[TERMINAL_COMMAND_NUMBER] = {
    {terminal_draw_pony,    command_pony,   TERMINAL_ARG_NONE},
    {timer_tim1_dly_sec,    command_dly,    TERMINAL_ARG_INT },
    {terminal_info_message, command_msg,    TERMINAL_ARG_STR },
    {heap_stat,             command_hstat,  TERMINAL_ARG_NONE},
    {terminal_sensor_data,  command_sensor, TERMINAL_ARG_STR },
    {peon_stat,             command_pstat,  TERMINAL_ARG_NONE},
    {terminal_output_logs,  command_lout,   TERMINAL_ARG_NONE},
    {terminal_clear_logs,   command_lclr,   TERMINAL_ARG_NONE},
    {log_add,               command_ladd,   TERMINAL_ARG_STR },
    {esp_send_cmd,          command_espcmd, TERMINAL_ARG_INT },
    {terminal_help,         command_help,   TERMINAL_ARG_NONE}
};

/**
 * buffer for storing terminal command
 */
static char terminal_input_buffer[TERMINAL_INPUT_BUFFER_SIZE];

/**
 * terminal buffer character counter
 */
static uint32_t input_buffer_counter = 0;

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
    while (1) {
        if (receiver_available(STDIO)) {
            terminal_busy = true;
            terminal_input_buffer[input_buffer_counter] = get_char(STDIO);
            if (terminal_input_buffer[input_buffer_counter] != BACKSPACE
                || input_buffer_counter > 0) {
                put_char(STDIO, terminal_input_buffer[input_buffer_counter]);
            }

            if (terminal_input_buffer[input_buffer_counter] ==
                BACKSPACE) {
                if(input_buffer_counter > 0) {
                    input_buffer_counter--;
                }
            } else if (input_buffer_counter >= TERMINAL_INPUT_BUFFER_SIZE) {
                terminal_clear_input_buffer();
                input_buffer_counter = 0;
                put_newline(STDIO);
                terminal_error_message("Input bufer overflow.");
                put_string(STDIO, "> ");
            } else if (terminal_input_buffer[input_buffer_counter] == NEWLINE) {
                if (input_buffer_counter != 0 && 
                        !terminal_process_command(input_buffer_counter)) {
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

bool terminal_process_command(const uint32_t input_buffer_counter) {
    //! error flag
    bool retval = false;
    //! create arguments list
    list_str *command = list_create_head();
    if (!command) {
        goto end;
    }
    list_str *arg_tmp = command;

    //! get the first argument (command)
    char *tokenizer = strtok(terminal_input_buffer," \r\n");

    //! parsing arguments
    while (tokenizer != NULL) {
        if (!list_str_write(arg_tmp, tokenizer)) {
            goto end;
        }
        tokenizer = strtok (NULL, " \r\n");
        if (tokenizer) {
            arg_tmp->ptr = list_str_new_entry(command);
            if (!arg_tmp->ptr) {
                goto end;
            }
            arg_tmp = arg_tmp->ptr;
        }
    }

    for (int i = 0; i < TERMINAL_COMMAND_NUMBER; ++i) {
        if ((strncmp(command->str, terminal_command_list[i].
            terminal_command_string, strlen(command->str)) == 0) &&
                strlen(command->str) ==
                    strlen(terminal_command_list[i].terminal_command_string)) {
            //! get the second argument (first argument to be passed to handler)
            arg_tmp = command->ptr;
            switch (terminal_command_list[i].terminal_command_arg) {
                case TERMINAL_ARG_NONE:
                    if (arg_tmp) {
                        goto end;
                    }
                    terminal_command_list[i].terminal_command_function();
                    break;
                case TERMINAL_ARG_INT:
                    if (!arg_tmp || arg_tmp->ptr) {
                        goto end;
                    }
                    terminal_command_list[i].
                        terminal_command_function(atoi(arg_tmp->str));
                    break;
                case TERMINAL_ARG_STR:
                    if (!arg_tmp || arg_tmp->ptr) {
                        goto end;
                    }
                    terminal_command_list[i].
                        terminal_command_function(arg_tmp->str);
                    break;
            }
            retval = true;
            goto end;
        }
    }
    end :
    //! free memory allocated for argument list
    list_str_delete_by_head(command);
    return retval;
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

void terminal_clear_input_buffer(void) {
    for (int i = 0; i < TERMINAL_INPUT_BUFFER_SIZE; ++i) {
        terminal_input_buffer[i] = '\0';
    }
}

void terminal_printf(const char *fmt, ...) {
    va_list va;
    va_start(va, fmt);

    char buf[STR_BUF_SIZE];
    char ch;

    while ((ch = *(fmt++))) {
        if (ch != '%') {
            put_char(STDIO, ch);
        } else {
            char zero_pad = 0;
            char *ptr;
            unsigned int len;

            ch = *(fmt++);

            if (ch == '0') {
                ch = *(fmt++);
                if (ch == '\0') {
                    goto end;
                }
                if (ch >= '0' && ch <= '9') {
                    zero_pad = ch - '0';
                }
                ch = *(fmt++);
            }

            switch (ch) {
                case 0:
                    goto end;
                case 'u':
                case 'd':
                    len = itoa(va_arg(va, unsigned int), 10, 0, (ch == 'u'),
                        buf, zero_pad);
                    nput_string(STDIO, buf, len);
                    break;
                case 'x':
                case 'X':
                    len = itoa(va_arg(va, unsigned int), 16, (ch == 'X'), 1,
                        buf, zero_pad);
                    nput_string(STDIO, buf, len);
                    break;
                case 'c' :
                    put_char(STDIO, (char)(va_arg(va, int)));
                    break;
                case 's' :
                    ptr = va_arg(va, char*);
                    nput_string(STDIO, ptr, strlen(ptr));
                    break;
                default:
                    put_char(STDIO, ch);
                    break;
            }
        }
    }

    end:

    put_newline(STDIO);

    va_end(va);
}

void terminal_sensor_data(const char *sensor_name) {
    if (strncmp(sensor_name, sensor_dht, strlen(sensor_dht)) == 0 &&
        strlen(sensor_dht) == strlen(sensor_name)) {
        terminal_printf("DHT sensor:");
        terminal_printf("temperature:   %d C\r"
                        "humidity:      %d %%",
                        dht_get_temperature(),
                        dht_get_humidity());
    } else {
        terminal_error_message("Invalid sensor name");
        terminal_printf("Available sensors:\r"
                        "%s",
                        sensor_dht);
    }
}

void terminal_output_logs(void) {
    context_lock();
    log_start_read();
    while(1) {
        char *message = log_get();
        if (message) {
            terminal_info_message(message);
        } else {
            break;
        }
    }
    context_unlock();
}

void terminal_clear_logs(void) {
    log_clear();
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
                    "%s [sensor name]:\r"
                    "sensor data\r\r"
                    "%s :\r"
                    "thread statistics\r\r"
                    "%s :\r"
                    "output system logs\r\r"
                    "%s :\r"
                    "clear system log list\r\r"
                    "%s [log message] :\r"
                    "add system log\r\r"
                    "%s [command] :\r"
                    "send command to connected esp8266\r\r"
                    "%s :\r"
                    "output this message",
                    command_pony,
                    command_dly,
                    command_msg,
                    command_hstat,
                    command_sensor,
                    command_pstat,
                    command_lout,
                    command_lclr,
                    command_ladd,
                    command_espcmd,
                    command_help);
}
