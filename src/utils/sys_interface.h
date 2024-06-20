/*
 * Created by french on 2/3/24.
*/

#ifndef MINI_APP_OS_INTERFACE_H
#define MINI_APP_OS_INTERFACE_H

#include <stdbool.h>
#include "comm.h"

RVALUE set_display_env(void);

void get_timestamp(char *buf, size_t bufsz);

#endif  /* MINI_APP_OS_INTERFACE_H */
