#ifndef CUTIL_H
#define CUTIL_H

#include "lib/list.h"

/**
 * @brief      heap memory utility handler
 *
 * @param[in]  args    cmd args container
 *
 * @return     status code
 */
int cutil_cmd_handler(list_iface *args);

#endif
