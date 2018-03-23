#ifndef LCD_H
#define LCD_H

#include "common/common.h"
#include "lib/list.h"

/**
 * @brief      LCD terminal command handler
 *
 * @param[in]  args    cmd args container
 *
 * @return     status code
 */
int lcd_cmd_handler(list_iface *args);

#endif
