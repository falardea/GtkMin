/*
 * Created by french on 3/10/24.
*/
#ifndef MINI_APP_VIEW_MSGOUT_H
#define MINI_APP_VIEW_MSGOUT_H
#include <stdarg.h>
#include <gtk/gtk.h>
#include "comm.h"

#ifdef __cplusplus
extern "C" {
#endif

void print_log_level_msgout(LOGLEVEL loglevel, const char *_format, ...);

#ifdef __cplusplus
}  /* closing brace for extern "C" */
#endif
#endif  /* MINI_APP_VIEW_MSGOUT_H */
