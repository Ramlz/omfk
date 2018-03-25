#include "utils/sensor.h"
#include "driver/dht.h"
#include "lib/string.h"
#include "peons/sensors.h"

#define SENSOR_CMD_SHOW    "show"
#define SENSOR_CMD_PERIOD  "period"
#define SENSOR_CMD_DISABLE "disable"
#define SENSOR_CMD_ENABLE  "enable"
#define SENSOR_CMD_STAT    "stat"

#define SENSOR_NAME_DHT "dht"

int sensor_cmd_handler(list_iface *args) {
    if (args->size(args) > 3) {
        return -1;
    }

    char *arg = args->get(args, 1);
    if (!arg) {
        return -1;
    }

    if (strcmp(arg, SENSOR_CMD_SHOW) == 0) {
        arg = args->get(args, 2);
        if (!arg) {
            return -1;
        }

        if (strcmp(arg, SENSOR_NAME_DHT) == 0) {
            dht_iface *iface_dht = dht_iface_get();
            if (iface_dht) {
                printf("temperature:   %d C\n"
                    "humidity:      %d %%\n",
                        iface_dht->get_temperature(iface_dht),
                            iface_dht->get_humidity(iface_dht));
            } else {
                printf("DHT driver error\n");
            }
            return 0;
        }
    } else if (strcmp(arg, SENSOR_CMD_PERIOD) == 0) {
        arg = args->get(args, 2);
        if (arg) {
            sensors_set_period(atoi(arg));
        } else {
            printf("sensors read period:   %d\n", sensors_get_period());
        }
        return 0;
    } else if (strcmp(arg, SENSOR_CMD_DISABLE) == 0) {
        arg = args->get(args, 2);
        if (!arg) {
            return -1;
        }

        sensors_set_enabled(SENSOR_NAME_DHT, false);
        return 0;
    } else if (strcmp(arg, SENSOR_CMD_ENABLE) == 0) {
        arg = args->get(args, 2);
        if (!arg) {
            return -1;
        }

        sensors_set_enabled(SENSOR_NAME_DHT, true);
        return 0;
    } else if (strcmp(arg, SENSOR_CMD_STAT) == 0) {
        arg = args->get(args, 2);
        if (!arg) {
            return -1;
        }

        sensors_print_stat(arg);
        return 0;
    }

    return -1;
}
