/*
 * Created by falar on 2/24/2024.
*/
#ifndef MINI_APP_LOGGING_H
#define MINI_APP_LOGGING_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include "../comm.h"

// #define LOG_INFO(format, ...) logging_llprintf(LOGLEVEL_INFO, "%s: %s\n", __func__, format __VA_OPT__(,) __VA_ARGS__)
// #define LOG_DEBUG(format, ...) logging_llprintf(LOGLEVEL_DEBUG, "%s: %s\n", __func__, format __VA_OPT__(,) __VA_ARGS__)
// #define LOG_ERROR(format, ...) logging_llprintf(LOGLEVEL_ERROR, "%s: %s\n", __func__, format __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief get the given log level as a printable string
 * @param level the log level string to return
 * @return a pointer to a string (const char*) representing the log level
 */
const char *get_log_level_str(LOGLEVEL level);

/**
 * @brief a wrapper around vsnprintf that prints a log message to the screen based on the application log level, and
 * may or may not print a timestamp prefix, depending on the application log_with_timestamps flag.
 * @param level the log level the caller has declared for this message
 * @param _format a printf style format string
 * @param ... a varg list of variables to be expanded into the _format string
 * @return a negative number if an error occurred, or the number of characters that would have been written, had
 * LOGGING_MAX_MSG_LENGTH been long enough to accommodate the string length (which is just a truncation problem),
 * or zero on success
 */
int logging_llprintf(LOGLEVEL level, const char *_format, ...);

#ifdef __cplusplus
}  /* closing brace for extern "C" */
#endif
#endif  /* MINI_APP_LOGGING_H */
