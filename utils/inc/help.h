#ifndef HELP_H
#define HELP_H

#include "lib/list.h"

/**
 * @brief      output terminal help
 *
 * @param      args  cmd args container
 *
 * @return     status code
 */
int help_cmd_handler(list_iface *args);

#endif
