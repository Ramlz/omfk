#include "utils/echo.h"
#include "lib/string.h"

int echo_cmd_handler(list_iface *args) {
    int size = args->size(args);
    if (size != 2) {
        return -1;
    }

    printf("%s\n",args->get(args, 1));

    return 0;
}
