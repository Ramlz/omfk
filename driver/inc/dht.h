#ifndef DHT_H
#define DHT_H

#include "timer.h"
#include "gpio.h"
#include "log.h"
#include "clock.h"

#define DHT_GPIO_PORT GPIO_C
#define DHT_GPIO_PIN  7

#define DHT_MAX_FAILURES 5

/**
 * @defgroup DHT_RESPONSES dht sensor response codes
 *
 * @{
 */

/**
 * no error
 */
#define DHT_OK               0
/**
 * bad second checksum
 */
#define DHT_BAD_SUM          1
/**
 * receiving bits timeout
 */
#define DHT_TIMEOUT          2

/** @} */

/**
 * @brief      initialize GPIO pin (defaults to PC7)
 */
void dht_init(void);
/**
 * @brief      read data from sensor
 * NOTE: data shouldn't be read more frequent than once per second
 *
 * @return     error code
 * @retval     DHT_OK         no error
 * @retval     DHT_BAD_SUM    data received but corrupted
 * @retval     DHT_TIMEOUT    data receiving timeout
 */
uint8_t dht_read(void);
/**
 * @brief      get temperature data
 * NOTE: data should be read first
 *
 * @return     temperature value
 */
uint8_t dht_get_temperature(void);
/**
 * @brief      get humidity data
 * NOTE: data should be read first
 *
 * @return     humidity value
 */
uint8_t dht_get_humidity(void);

/**
 * @brief      periodicaly read DHT sensor data
 */
void dht_task(void);

#endif
