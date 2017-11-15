#include "dht.h"

static uint8_t  humidity = 0;
static uint8_t  temperature = 0;

void dht_init(void) {
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
        timer_tim1_dly_usec(5);
        timeout += 5;
        if (timeout > 100) {
            return DHT_TIMEOUT;
        }
    }

    timeout = 0;
    while (gpio_read(DHT_GPIO_PORT, DHT_GPIO_PIN)) {
        timer_tim1_dly_usec(5);
        timeout += 5;
        if (timeout > 100) {
            return DHT_TIMEOUT;
        }
    }

    for (int i = 0; i < 40; i++) {
        timeout = 0;
        while(!gpio_read(DHT_GPIO_PORT, DHT_GPIO_PIN)) {
            timer_tim1_dly_usec(5);
            timeout += 5;
            if (timeout > 100) {
                return DHT_TIMEOUT;
            }
        }

        timeout = 0;
        while(gpio_read(DHT_GPIO_PORT, DHT_GPIO_PIN)) {
            timer_tim1_dly_usec(5);
            timeout += 5;
            if (timeout > 100) {
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

    humidity = data[0];
    temperature = data[2];

    uint8_t sum = data[0] + data[1] + data[2] + data[3];

    return (sum == data[4]) ? DHT_OK : DHT_BAD_SUM;
}

uint8_t dht_get_temperature(void) {
    return temperature;
}

uint8_t dht_get_humidity(void) {
    return humidity;
}
