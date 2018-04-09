#include "utils/led.h"
#include "peons/led.h"
#include "lib/string.h"

#define LED_CMD_ENABLE  "enable"
#define LED_CMD_DISABLE "disable"

int led_cmd_handler(list_iface *args) {
    if (args->size(args) != 2) {
        return -1;
    }

    char *arg = args->get(args, 1);

    if (strcmp(arg, LED_CMD_ENABLE) == 0) {
        led_enable(true);
        return 0;
    } else if (strcmp(arg, LED_CMD_DISABLE) == 0) {
        led_enable(false);
        return 0;
    }

    return -1;
}
