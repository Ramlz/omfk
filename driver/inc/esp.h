#ifndef ESP_H
#define ESP_H

#define ESP_USART USART_1

#define ESP_TIMEOUT_MS 500

/**
 * @brief      general initilization
 */
void esp_init(void);

/**
 * @brief      initializes usart connected to esp
 */
void esp_init_usart(void);

/**
 * @brief      send AT command to esp
 *
 * @param[in]  cmd_id  command identifier
 */
void esp_send_cmd(int cmd_id);

#endif
