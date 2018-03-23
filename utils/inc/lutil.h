#ifndef LUTIL_H
#define LUTIL_H

#include "lib/list.h"

/**
 * @brief      log control utility
 *
 * @param[in]  args    cmd args container
 *
 * @return     status code
 */
int lutil_cmd_handler(list_iface *args);

#endif
