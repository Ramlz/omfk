#ifndef DELAY_H
#define DELAY_H

#include "lib/list.h"

/**
 * @brief      delay for a specified amount of time
 *
 * @param[in]  args    cmd args container
 *
 * @return     status code
 */
int delay_cmd_handler(list_iface *args);

#endif
