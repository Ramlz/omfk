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

/**
 * @brief      array of command contexts
 */
terminal_command_context terminal_command_list[TERMINAL_COMMAND_NUMBER] = {
    {terminal_draw_pony, "pony", TERMINAL_ARG_NONE},
    {clock_delay_seconds, "dly", TERMINAL_ARG_INT},
    {terminal_info_message, "msg", TERMINAL_ARG_STR}
};

/**
 * buffer for storing terminal command
 */
static char terminal_input_buffer[TERMINAL_INPUT_BUFFER_SIZE];

void terminal_init(void) {
    init_usart(USART_2, DEFAULT_BAUD_RATE);
}

void terminal_start(void) {
    put_string(USART_2, "$ ");
    uint32_t input_buffer_counter = 0;
    while (1) { 
        if (receiver_available(USART_2)) {
            terminal_input_buffer[input_buffer_counter] = get_char(USART_2);
            if (terminal_input_buffer[input_buffer_counter] != BACKSPACE
                || input_buffer_counter > 0) {
                put_char(USART_2, terminal_input_buffer[input_buffer_counter]);
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
                put_string(USART_2, "$ ");
            } else {
                input_buffer_counter++;
            }
        }
    }
}

bool terminal_process_command(const uint32_t input_buffer_counter) {
    char command[TERMINAL_ARG_MAX_SIZE];
    char str_arg[TERMINAL_ARG_MAX_SIZE];
    unsigned int int_arg = 0;

    sscanf(terminal_input_buffer, "%s %s", command, str_arg);
    sscanf(str_arg, "%d", &int_arg);

    for (int i = 0; i < TERMINAL_COMMAND_NUMBER; ++i) {
        if (strncmp(command, terminal_command_list[i].
            terminal_command_string, strlen(terminal_command_list[i].
                terminal_command_string)) == 0) {
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
            return true;
        }
    }
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
        put_string(USART_2, "[AUHTUNG] ");
    }
    put_line(USART_2, message);
}

void terminal_draw_pony(void) {
    put_string(USART_2, pony);
}

void terminal_clear_input_buffer(void) {
    for (int i = 0; i < TERMINAL_INPUT_BUFFER_SIZE; ++i) {
        terminal_input_buffer[i] = ' ';
    }
}
