#include "utils/sensor.h"
#include "driver/dht.h"
#include "lib/string.h"

#define SENSOR_CMD_SHOW "show"

#define SENSOR_NAME_DHT "dht"

int sensor_cmd_handler(list_iface *args) {
    int size = args->size(args);
    if (size != 3) {
        return -1;
    }

    char *arg = args->get(args, 1);

    if (strcmp(arg, SENSOR_CMD_SHOW) == 0) {
        arg = args->get(args, 2);

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
    }

    return -1;
}
