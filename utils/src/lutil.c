#include "utils/lutil.h"
#include "peons/log.h"

#define LUTIL_CMD_ADD "add"

int lutil_cmd_handler(list_iface *args) {
    int size = args->size(args);
    if (size != 3) {
        return -1;
    }

    char *arg = args->get(args, 1);
    if (strcmp(arg, LUTIL_CMD_ADD) == 0) {
        log_add(arg = args->get(args, 2));
        return 0;
    }

    return -1;
}
