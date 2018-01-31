#include "sensor.h"
#include "dht.h"

/**
 * sensor names
 */
static const char sensor_dht[]         = "dht";

void sensor_data(const char *sensor_name) {
    if (strcmp(sensor_name, sensor_dht) == 0) {
        terminal_printf("DHT sensor:\r");
        terminal_printf("temperature:   %d C\r"
                        "humidity:      %d %%\r",
                        dht_get_temperature(),
                        dht_get_humidity());
    } else {
        terminal_error_message("Invalid sensor name");
        terminal_printf("Available sensors:\r"
                        "%s\r",
                        sensor_dht);
    }
}
