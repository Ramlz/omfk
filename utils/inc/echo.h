#ifndef ECHO_H
#define ECHO_H

#include "lib/list.h"

/**
 * @brief      printint utility handler
 *
 * @param[in]  args    cmd args container
 *
 * @return     status code
 */
int echo_cmd_handler(list_iface *args);

#endif
