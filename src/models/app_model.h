/*
 * Created by french on 2/3/24.
*/
#ifndef MINI_APP_APP_MODEL_H
#define MINI_APP_APP_MODEL_H
#include <stdbool.h>
#include "comm.h"
#include "utils/logging.h"

G_BEGIN_DECLS // C++ guard, from gmacros.h

typedef enum {
   INIT_UNINITIALIZED,
   INIT_STARTED,
   INIT_PARSE_ARGS_COMPLETE,
   INIT_SET_ENV_COMPLETE,
   INIT_GTK_INIT_COMPLETE,
   INIT_APP_BUILDER_COMPLETE,
   INIT_APP_STYLING_COMPLETE,
   INIT_SUCCESS,
   INIT_ERROR,
   INIT_UNKNOWN_STATE
} APP_INIT_STATE;

typedef enum {
   RUNTIME_UNINITIALIZED,
   RUNTIME_INITIALIZING,
   RUNTIME_RUNNING_NORMALLY,
   RUNTIME_HANDLING_ERROR,
   RUNTIME_RUNNING_DEPRECATED,
   RUNTIME_RUNNING_SAFE_MODE,
   RUNTIME_SHUTTING_DOWN
} APP_RUNTIME_STATE;

typedef struct {
   APP_INIT_STATE init_state;
   APP_RUNTIME_STATE runtime_state;
   bool debugging_enabled;
   bool log_with_timestamps;
   LOGLEVEL runtime_log_level;
} app_model_t;

/**
 * @brief get the application initialization state as a printable string
 * @param state the initialization state string to return
 * @return a pointer to a string (const char*) representing the initialization state the caller passed in
 */
const char *get_init_state_str(APP_INIT_STATE state);
/**
 * @brief get the application runtime state as a printable string
 * @param state the runtime state string to return
 * @return a pointer to a string (const char*) representing the runtime state the caller passed in
 */
const char *get_runtime_state_str(APP_RUNTIME_STATE state);

/**
 * @brief set the application initialization state
 * @param state the state to set on the app model
 */
void set_app_init_state(APP_INIT_STATE state);
/**
 * @brief get the current application initialization state
 * @return the current application initialization state
 */
APP_INIT_STATE get_app_init_state(void);

/**
 * @brief set the application runtime state
 * @param state the state to set on the app model
 */
void set_app_runtime_state(APP_RUNTIME_STATE state);
/**
 * @brief get the current application runtime state
 * @return the current application runtime state
 */
APP_RUNTIME_STATE get_app_runtime_state(void);

/**
 * @brief set the application debugging_enabled flag, for enabling/disabling debugging/maintenance features
 * @param enable the state to set debugging_enabled
 */
void set_app_debug_flag(bool enable);
/**
 * @brief get the state of the application debugging_enabled flag
 * @return true if the debugging_enabled flag is set, false otherwise
 */
bool get_app_debug_flag(void);

/**
 * @brief set the application log_with_timestamps flag, for enabling/disabling timestamps prefixing log messages
 * @param enable the state to set log_with_timestamps
 */
void set_app_log_w_timestamp_flag(bool enable);
/**
 * @brief get the state of the application log_with_timestamps flag
 * @return true if the log_with_timestamps flag is set, false otherwise
 */
bool get_app_log_w_timestamp_flag(void);

/**
 * @brief set the application runtime_log_level.  This will not raise the application runtime_log_level above the
 * default, but will lower it
 * @param enable true to turn on application runtime_log_level
 */
void set_app_log_level(LOGLEVEL logLevel);
/**
 * @brief get the application runtime_log_level
 * @return the application runtime_log_level
 */
LOGLEVEL get_app_log_level(void);

G_END_DECLS
#endif  /* MINI_APP_APP_MODEL_H */
