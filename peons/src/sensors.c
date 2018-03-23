#include "peons/sensors.h"
#include "driver/dht.h"
#include "board/cfg.h"
#include "peons/log.h"
#include "kernel/peons.h"
#include "platform/clock.h"

#define SENSOR_MAX_FAILURES 10
#define SENSOR_READ_RATE    2

static const dht_config dht_sensor_config = {
    .port  = DHT_GPIO_PORT,
    .pin   = DHT_GPIO_PIN,
    .timer = DHT_TIMER
};

void sensor_loop(void) {
    dht_init(&dht_sensor_config);

    dht_iface *iface_dht = dht_iface_get();
    if (!iface_dht) {
        while(true);
    }

    uint32_t dht_failure_counter = 0;

    while (true) {
        peon_lock();

        if (!iface_dht->read(iface_dht)) {
            dht_failure_counter++;
        } else {
            dht_failure_counter = 0;
        }

        peon_unlock();

        if (dht_failure_counter > SENSOR_MAX_FAILURES) {
            log_add("[DHT] too many consequent failures");
            dht_failure_counter = 0;
        }

        clock_dly_secs(SENSOR_READ_RATE);
    }
}
