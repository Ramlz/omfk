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
"     -  .' :' : /   / _( \\  `.(   `.  `;.  \\  \\\r"
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

char command_pony[]  = "pony";
char command_dly[]   = "dly";
char command_msg[]   = "msg";
char command_hstat[] = "hstat";

/**
 * @brief      array of command contexts
 */
terminal_command_context terminal_command_list[TERMINAL_COMMAND_NUMBER] = {
    {terminal_draw_pony,    command_pony,  TERMINAL_ARG_NONE},
    {clock_delay_seconds,   command_dly,   TERMINAL_ARG_INT},
    {terminal_info_message, command_msg,   TERMINAL_ARG_STR},
    {heap_stat,             command_hstat, TERMINAL_ARG_NONE}
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
    put_string(STDIO, "$ ");
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
            } else if (terminal_input_buffer[input_buffer_counter] == NEWLINE) {
                if (!terminal_process_command(input_buffer_counter)) {
                    terminal_error_message("invalid command");
                }
                terminal_clear_input_buffer();
                input_buffer_counter = 0;
                put_string(STDIO, "$ ");
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

    tokenizer = strtok(terminal_input_buffer," \r");
    command = cell_alloc(strsize(tokenizer));
    if (!command) {
        return false;
    }
    strcpy(command, tokenizer);

    tokenizer = strtok(NULL," \r");
    str_arg = cell_alloc(strsize(tokenizer));
    if (!str_arg) {
        cell_free(command);
        return false;
    }
    strcpy(str_arg, tokenizer);

    int_arg = atoi(tokenizer);
    for (int i = 0; i < TERMINAL_COMMAND_NUMBER; ++i) {
        if ((strncmp(command, terminal_command_list[i].terminal_command_string,
            strlen(command)) == 0) && strlen(command) ==
                strlen(terminal_command_list[i].terminal_command_string)) {
            switch (terminal_command_list[i].terminal_command_arg) {
                case TERMINAL_ARG_NONE:
                    terminal_command_list[i].terminal_command_function();
                    break;
                case TERMINAL_ARG_INT:
                    terminal_command_list[i].terminal_command_function(int_arg);
                    break;
                case TERMINAL_ARG_STR:
                    terminal_command_list[i].terminal_command_function(str_arg);
                    break;
            }
            cell_free(command);
            cell_free(str_arg);
            return true;
        }
    }
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
        terminal_input_buffer[i] = ' ';
    }
}

void terminal_printf(const char *fmt, ...) {
    va_list va;
    va_start(va, fmt);

    char buf[STR_BUF_SIZE];
    char ch;

    while ((ch=*(fmt++))) {
        if (ch!='%') {
            put_char(STDIO, ch);
        } else {
            char zero_pad = 0;
            char *ptr;
            unsigned int len;

            ch=*(fmt++);

            if (ch=='0') {
                ch=*(fmt++);
                if (ch == '\0') {
                    goto end;
                }
                if (ch >= '0' && ch <= '9') {
                    zero_pad = ch - '0';
                }
                ch=*(fmt++);
            }

            switch (ch) {
                case 0:
                    goto end;
                case 'u':
                case 'd':
                    len = itoa(va_arg(va, unsigned int), 10, 0, (ch=='u'),
                        buf, zero_pad);
                    nput_string(STDIO, buf, len);
                    break;
                case 'x':
                case 'X':
                    len = itoa(va_arg(va, unsigned int), 16, (ch=='X'), 1,
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
