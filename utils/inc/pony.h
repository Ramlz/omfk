#ifndef PONY_H
#define PONY_H

#include "lib/list.h"

/**
 * @brief      output pony ASCII art to standart output
 *
 * @param[in]  args    cmd args container
 *
 * @return     status code
 */
int pony_cmd_handler(list_iface *args);

#endif
