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
 * @brief      array of command contexts
 */
terminal_command_context terminal_command_list[TERMINAL_COMMAND_NUMBER] = {
    {terminal_draw_pony,    command_pony,   TERMINAL_ARG_NONE},
    {timer_tim1_dly_sec,    command_dly,    TERMINAL_ARG_INT },
    {terminal_info_message, command_msg,    TERMINAL_ARG_STR },
    {heap_stat,             command_hstat,  TERMINAL_ARG_NONE},
    {terminal_sensor_data,  command_sensor, TERMINAL_ARG_NONE},
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

void terminal_init(void) {
    // initialize STDIO
    init_usart(STDIO, DEFAULT_BAUD_RATE);
}

void terminal_start(void) {
    put_string(STDIO, "> ");
    uint32_t input_buffer_counter = 0;
    while (1) {
        if (receiver_available(STDIO)) {
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
                if (!terminal_process_command(input_buffer_counter)) {
                    terminal_error_message("Invalid command. Type \"help\""
                        " for list of available commands.");
                }
                terminal_clear_input_buffer();
                input_buffer_counter = 0;
                put_string(STDIO, "> ");
            } else {
                input_buffer_counter++;
            }
        }
    }
}

bool terminal_process_command(const uint32_t input_buffer_counter) {
    char *command = NULL;
    char *str_arg = NULL;
    unsigned int int_arg = 0;

    char *tokenizer = NULL;

    tokenizer = strtok(terminal_input_buffer," \r\n");
    command = cell_alloc(strsize(tokenizer));
    if (!command) {
        goto invalid;
    }
    strcpy(command, tokenizer);

    tokenizer = strtok(NULL," ");

    str_arg = cell_alloc(strsize(tokenizer));
    if (!str_arg) {
        goto invalid;
    }
    strcpy(str_arg, tokenizer);

    int_arg = atoi(tokenizer);

    for (int i = 0; i < TERMINAL_COMMAND_NUMBER; ++i) {
        if ((strncmp(command, terminal_command_list[i].terminal_command_string,
            strlen(command)) == 0) && strlen(command) ==
                strlen(terminal_command_list[i].terminal_command_string)) {
            switch (terminal_command_list[i].terminal_command_arg) {
                case TERMINAL_ARG_NONE:
                    if (str_arg[0]) {
                        goto invalid;
                    }
                    terminal_command_list[i].terminal_command_function();
                    break;
                case TERMINAL_ARG_INT:
                    if (strtok(NULL," \r\n")) {
                        goto invalid;
                    }
                    terminal_command_list[i].terminal_command_function(int_arg);
                    break;
                case TERMINAL_ARG_STR:
                    if (strtok(NULL," \r\n")) {
                        goto invalid;
                    }
                    terminal_command_list[i].terminal_command_function(str_arg);
                    break;
            }
            cell_free(command);
            cell_free(str_arg);
            return true;
        }
    }
    invalid :
    cell_free(command);
    cell_free(str_arg);
    return false;
}

void terminal_info_message(const char *message) {
    terminal_message(message, false);
}

void terminal_error_message(const char *message) {
    terminal_message(message, true);
}

void terminal_message(const char *message, const bool error) {
    // check if it's an error message
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

void terminal_sensor_data(void) {
    terminal_printf("DHT sensor:");
    terminal_printf("temperature:   %d C\r"
                    "humidity:      %d %%",
                    dht_get_temperature(),
                    dht_get_humidity());
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
                    "%s :\r"
                    "sensor data\r\r"
                    "%s :\r"
                    "thread statistics\r\r"
                    "%s :\r"
                    "output system logs\r\r"
                    "%s :\r"
                    "clear system log list\r\r"
                    "%s [string] :\r"
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
