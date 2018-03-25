#include "utils/pony.h"
#include "lib/string.h"

/**
 * just a string with pony...nuff said
 */
static const char pony_str[] =
"                                     __         __----__\n"
"                                    /  \\__..--'' `-__-__''-_\n"
"                                   ( /  \\    ``--,,  `-.''''`\n"
"                                   | |   `-..__  . \\    `.\n"
"                                   ( '. \\ _____`\\ )`-_    `.\n"
"                                   '\\  __/   __\\ ' / `:-.._\\\n"
"                                   .|\\_ (   / .-|  |-.|    ``'\n"
"                                   |' (  \\ ( (WW|  \\W)j\n"
"      ..---'''':----,             .||  ', \\_\\_`_|   ``-.\n"
"    ,'      .'` .'``-`,___        |''.   `.        \\__/\n"
"   /   _  .'  :'   _ `|.`.`-.     |   \\ ._:7,______.-'\n"
"  | .-'/  : .'  .-' `. \\`.`. `-.__':   `-\\    /\n"
"  '`  /  :' : .: .-''>-'\\ `.`-.(`-| '.    |  (\n"
"     .' :' : /   / _( \\  `.(   `.  `;.  \\  \\\n"
"     |  | .' : /|  | (____`. (     |\\   )\\  ;  |\n"
"    .' .' | | | `. |   \\  `.___.-' ) /  )/   |\n"
"    |  |  | | |  | |   ///           |/   '    |\n"
"   .' .'  '.'.`; |/ \\  /     /             \\__/\n"
"   |  |    | | |.|   |      /-,_______\\       \\\n"
"  /  / )   | | '|' _/      /     |    |\\       \\\n"
".:.-' .'  .' |   )/       /     |     | `--,    \\\n"
"     /    |  |  / |      |      |     |   /      )\n"
".__.'    /`  :|/_/|      |      |      | (       |\n"
"`-.___.-`;  / '   |      |      |      |  \\      |\n"
"       .:_-'      |       \\     |       \\  `.___/\n"
"                   \\_______)     \\_______)\n";

int pony_cmd_handler(list_iface *args) {
    int size = args->size(args);
    if (size == 1) {
        printf(pony_str);
        return 0;
    }

    return -1;
}
