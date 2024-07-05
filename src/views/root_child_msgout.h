/*
 * Created by french on 3/10/24.
*/
#ifndef MINI_APP_VIEW_MSGOUT_H
#define MINI_APP_VIEW_MSGOUT_H
#include <stdarg.h>
#include "comm.h"

G_BEGIN_DECLS // C++ guard, from gmacros.h

void print_log_level_msgout(LOGLEVEL loglevel, const char *_format, ...);

G_END_DECLS
#endif  /* MINI_APP_VIEW_MSGOUT_H */
