#include "esp.h"
#include "usart.h"
#include "kernel_entry.h"

/**
 * esp AT commands
 */
static const char *at_commands[] = {
    //! Basic AT
    "AT\r\n",
    "AT+RST\r\n",
    "AT+GMR\r\n",
    "AT+GSLP\r\n",
    "ATE\r\n",
    //! WIFI Function AT
    "AT+CWMODE\r\n",
    "AT+CWJAP\r\n",
    "AT+CWLAP\r\n",
    "AT+CWQAP\r\n",
    "AT+CWSAP\r\n",
    "AT+CWLIF\r\n",
    "AT+CWDHCP\r\n",
    "AT+CIPSTAMAC\r\n",
    "AT+CIPAPMAC\r\n",
    "AT+CIPSTA\r\n",
    "AT+CIPAP\r\n",
    //! TCP/IP Function AT
    "AT+CIPSTATUS\r\n",
    "AT+CIPSTART\r\n",
    "AT+CIPSEND\r\n",
    "AT+CIPCLOSE\r\n",
    "AT+CIFSR\r\n",
    "AT+CIPMUX\r\n",
    "AT+CIPSERVER\r\n",
    "AT+CIPMODE\r\n",
    "AT+CIPSTO\r\n",
    "AT+CIUPDATE\r\n",
    "+IPD\r\n",
    "AT+IPR\r\n",
};

void esp_init(void) {
    esp_init_usart();
}

void esp_init_usart(void) {
    init_usart(ESP_USART, DEFAULT_BAUD_RATE);
}

void esp_send_cmd(int cmd_id) {
    context_lock();
    {
        put_string(ESP_USART, at_commands[cmd_id]);
        int timeout = clock_get();
        while (!strstr(get_usart_buf(ESP_USART), "OK\r\n") &&
            !strstr(get_usart_buf(ESP_USART), "ERROR\r\n") &&
                clock_get() < timeout + ESP_TIMEOUT_MS);
        usart_write_buf(ESP_USART, '\0');
        put_string(STDIO, get_usart_buf(ESP_USART));
        usart_clear_buf(ESP_USART);
    }
    context_unlock();
}
