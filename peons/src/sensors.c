#include "peons/sensors.h"
#include "driver/dht.h"
#include "board/cfg.h"
#include "peons/log.h"
#include "kernel/peons.h"
#include "platform/clock.h"
#include "platform/timer.h"

#define SENSOR_MAX_FAILURES 10

#define DEFAULT_SENSOR_PERIOD 2

#define SENSOR_NAME_DHT "dht"

/**
 * DHT sensor configuration
 */
static const dht_config dht_sensor_config = {
    .port  = DHT_GPIO_PORT,
    .pin   = DHT_GPIO_PIN,
    .timer = DHT_TIMER
};

/**
 * timer TIM1 configuration (used by DHT sensor)
 */
static const timer_config tim1_config = {
    .counter_mode = TIMER_COUNTER_MODE_UP,
    .prescaler    = 0x3f,
    .period       = 0xff
};

static uint32_t sensor_period = DEFAULT_SENSOR_PERIOD;

static bool dht_enabled = true;

void sensors_loop(void) {
    timer_init(TIM1, &tim1_config);

    dht_init(&dht_sensor_config);

    dht_iface *iface_dht = dht_iface_get();

    uint32_t dht_failure_counter = 0;

    while (true) {
        peon_lock();

        if (iface_dht && dht_enabled) {
            if (!iface_dht->read(iface_dht)) {
                dht_failure_counter++;
            } else {
                dht_failure_counter = 0;
            }
        }


        peon_unlock();

        if (dht_failure_counter > SENSOR_MAX_FAILURES) {
            log_add("[DHT] too many consequent failures, disabling sensor");
            dht_failure_counter = 0;
            dht_enabled = false;
        }

        clock_dly_secs(sensor_period);
    }
}

void sensors_set_period(uint32_t seconds) {
    sensor_period = seconds;
}

void sensors_set_enabled(char *sensor_name, bool enabled) {
    if (strcmp(sensor_name, SENSOR_NAME_DHT) == 0) {
        dht_enabled = enabled;
    }
}

uint32_t sensors_get_period(void) {
    return sensor_period;
}

void sensors_print_stat(char *sensor_name) {
    if (strcmp(sensor_name, SENSOR_NAME_DHT) == 0) {
        printf("[DHT]\n");
        printf("Enabled: ");
        if (dht_enabled) {
            printf("yes\n");
        } else {
            printf("no\n");
        }
    } else {
        error_message("Unknown sensor");
    }
}
