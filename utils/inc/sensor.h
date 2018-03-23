#ifndef SENSOR_H
#define SENSOR_H

#include "lib/list.h"

/**
 * @brief      sensor control utility
 *
 * @param[in]  args    cmd args container
 *
 * @return     status code
 */
int sensor_cmd_handler(list_iface *args);


#endif
