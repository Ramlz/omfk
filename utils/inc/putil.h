#ifndef PUTIL_H
#define PUTIL_H

#include "common/common.h"
#include "lib/list.h"

/**
 * @brief      thread utility handler
 *
 * @param[in]  args    cmd args container
 *
 * @return     status code
 */
int putil_cmd_handler(list_iface *args);

#endif
