#include "dht.h"

static uint8_t humidity = 0;
static uint8_t temperature = 0;

void dht_init(void) {
    timer_init(TIM1);
    // init GPIO pin connected to sensor
    gpio_init_pin(DHT_GPIO_PORT, DHT_GPIO_PIN, GPIO_PUSH_PULL, GPIO_OUTPUT,
        GPIO_HIGH_SPEED, GPIO_NO_PUPD, NULL);
}

uint8_t dht_read(void) {
    uint32_t timeout = 0;
    uint8_t data[5] = {0, 0, 0, 0, 0};
    uint8_t cnt = 7;
    uint8_t idx = 0;

    gpio_output(DHT_GPIO_PORT, DHT_GPIO_PIN);
    gpio_low(DHT_GPIO_PORT, DHT_GPIO_PIN);
    timer_tim1_dly_msec(18);
    gpio_high(DHT_GPIO_PORT, DHT_GPIO_PIN);
    timer_tim1_dly_usec(40);
    gpio_input(DHT_GPIO_PORT, DHT_GPIO_PIN);

    timeout = 0;
    while (!gpio_read(DHT_GPIO_PORT, DHT_GPIO_PIN)) {
        timer_tim1_dly_usec(2);
        timeout += 2;
        if (timeout > 100) {
            return DHT_TIMEOUT;
        }
    }

    timeout = 0;
    while (gpio_read(DHT_GPIO_PORT, DHT_GPIO_PIN)) {
        timer_tim1_dly_usec(2);
        timeout += 2;
        if (timeout > 100) {
            return DHT_TIMEOUT;
        }
    }

    for (int i = 0; i < 40; i++) {
        timeout = 0;
        while(!gpio_read(DHT_GPIO_PORT, DHT_GPIO_PIN)) {
            timer_tim1_dly_usec(2);
            timeout += 2;
            if (timeout > 100) {
                return DHT_TIMEOUT;
            }
        }

        timeout = 0;
        while(gpio_read(DHT_GPIO_PORT, DHT_GPIO_PIN)) {
            timer_tim1_dly_usec(2);
            timeout += 2;
            if (timeout > 100) {
                return DHT_TIMEOUT;
            }
        }

        if (timeout > 30) {
            data[idx] |= (1 << cnt);
        }
        if (cnt == 0) {
            cnt = 7;
            idx++;
        } else {
            cnt--;
        }
    }

    humidity = data[0];
    temperature = data[2];

    uint8_t sum = data[0] + data[1] + data[2] + data[3];

    return (sum == data[4] && sum != 0) ? DHT_OK : DHT_BAD_SUM;
}

uint8_t dht_get_temperature(void) {
    return temperature;
}

uint8_t dht_get_humidity(void) {
    return humidity;
}

void dht_task(void) {
    int failure_cnt = 0;
    int dht_error = DHT_OK;
    while (1) {
        clock_dly_secs(1);
        context_lock();
        {
            // read DHT sensor data
            dht_error = dht_read();
            if (dht_error != DHT_OK) {
                failure_cnt++;
                if (failure_cnt > DHT_MAX_FAILURES) {
                    switch (dht_error) {
                        case DHT_BAD_SUM:
                            log_add("[DHT] too many consequent failures,"
                                    " last error: DHT_BAD_SUM           ");
                            break;
                        case DHT_TIMEOUT:
                            log_add("[DHT] too many consequent failures,"
                                    " last error: DHT_TIMEOUT           ");
                            break;
                        default:
                            log_add("[DHT] too many consequent failures,"
                                    " last error: UNKNOWN               ");
                            break;
                    }
                }
            } else {
                failure_cnt = 0;
            }
        }
        context_unlock();
    }
}

