#include "dht.h"
#include "board_cfg.h"

static uint8_t humidity = 0;
static uint8_t temperature = 0;
static uint8_t dht_iterator = 0;

static const char bad_sum_err_str[] = "DHT_BAD_SUM";
static const char timeout_err_str[] = "DHT_TIMEOUT";
static const char unknown_err_str[] = "DHT_UNKNOWN";

/**
 * GPIO interface with DHT pin
 */
static gpio_iface *dht_gpio_iface = NULL;

void dht_init(void) {
    timer_init(TIM1);

    //! create GPIO port interface
    dht_gpio_iface = gpio_iface_get(DHT_GPIO_PORT);

    //! init GPIO pin connected to sensor
    dht_gpio_iface->init(dht_gpio_iface, DHT_GPIO_PIN,
        GPIO_OTYPE_PUSH_PULL, GPIO_MODE_OUTPUT, GPIO_SPEED_HIGH,
            GPIO_PUPD_NO, NULL);
}

uint8_t dht_read(void) {
    if (!dht_gpio_iface) {
        return DHT_UNKNOWN;
    }

    uint32_t timeout = 0;
    uint8_t data[5] = {0, 0, 0, 0, 0};
    uint8_t cnt = 7;
    uint8_t idx = 0;

    dht_iterator = 0;

    dht_gpio_iface->mode(dht_gpio_iface, DHT_GPIO_PIN, GPIO_MODE_OUTPUT);

    //! put pin to low for 18 msecs
    dht_gpio_iface->write(dht_gpio_iface, DHT_GPIO_PIN, false);
    timer_tim1_dly_msec(18);

    //! put pin to high for 40 usecs
    dht_gpio_iface->write(dht_gpio_iface, DHT_GPIO_PIN, true);
    timer_tim1_dly_usec(40);

    dht_gpio_iface->mode(dht_gpio_iface, DHT_GPIO_PIN, GPIO_MODE_INPUT);

    timeout = 0;
    while (!dht_gpio_iface->read(dht_gpio_iface, DHT_GPIO_PIN)) {
        timer_tim1_dly_usec(DHT_CYCLE_STEP);
        timeout += DHT_CYCLE_STEP;
        if (timeout > DHT_CYCLE_TIMEOUT) {
            return DHT_TIMEOUT;
        }
    }

    timeout = 0;
    while (dht_gpio_iface->read(dht_gpio_iface, DHT_GPIO_PIN)) {
        timer_tim1_dly_usec(DHT_CYCLE_STEP);
        timeout += DHT_CYCLE_STEP;
        if (timeout > DHT_CYCLE_TIMEOUT) {
            return DHT_TIMEOUT;
        }
    }

    for (dht_iterator = 0; dht_iterator < 40; dht_iterator++) {
        timeout = 0;
        while (!dht_gpio_iface->read(dht_gpio_iface, DHT_GPIO_PIN)) {
            timer_tim1_dly_usec(DHT_CYCLE_STEP);
            timeout += DHT_CYCLE_STEP;
            if (timeout > DHT_CYCLE_TIMEOUT) {
                return DHT_TIMEOUT;
            }
        }

        timeout = 0;
        while (dht_gpio_iface->read(dht_gpio_iface, DHT_GPIO_PIN)) {
            timer_tim1_dly_usec(DHT_CYCLE_STEP);
            timeout += DHT_CYCLE_STEP;
            if (timeout > DHT_CYCLE_TIMEOUT) {
                return DHT_TIMEOUT;
            }
        }

        if (timeout > 40) {
            data[idx] |= (1 << cnt);
        }
        if (cnt == 0) {
            cnt = 7;
            idx++;
        } else {
            cnt--;
        }
    }

    uint8_t sum = data[0] + data[1] + data[2] + data[3];

    if (sum == data[4] && sum != 0) {
        humidity = data[0];
        temperature = data[2];
        return DHT_OK;
    } else {
        return DHT_BAD_SUM;
    }
}

uint8_t dht_get_temperature(void) {
    return temperature;
}

uint8_t dht_get_humidity(void) {
    return humidity;
}

void dht_task(void) {
    const char *err_str = NULL;

    int failure_cnt = 0;
    int ret = DHT_OK;

    while (true) {
        clock_dly_secs(1);
        peon_lock();
        {
            //! read DHT sensor data
            ret = dht_read();
            if (ret != DHT_OK) {
                failure_cnt++;
                if (failure_cnt > DHT_MAX_FAILURES) {
                    switch (ret) {
                        case DHT_BAD_SUM:
                            err_str = bad_sum_err_str;
                            break;
                        case DHT_TIMEOUT:
                            err_str = timeout_err_str;
                            break;
                        case DHT_UNKNOWN:
                            err_str = unknown_err_str;
                            break;
                    }
                    log_add("[DHT] too many consequent failures,"
                        " last error: %s, cycles read: %d", err_str,
                        dht_iterator);
                    failure_cnt = 0;
                }
            } else {
                failure_cnt = 0;
            }
        }
        peon_unlock();
    }
}

