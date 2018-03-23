#include "utils/delay.h"
#include "platform/clock.h"
#include "lib/string.h"

int delay_cmd_handler(list_iface *args) {
    int size = args->size(args);
    if (size == 2) {
        int seconds = atoi(args->get(args, 1));
        if (seconds > 0) {
            clock_dly_secs(seconds);
            return 0;
        }
    }

    return -1;
}
