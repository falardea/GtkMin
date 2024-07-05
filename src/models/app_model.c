/*
 * Created by french on 2/3/24.
*/
#include <stdbool.h>
#include "app_model.h"

const LOGLEVEL DEFAULT_APP_LOG_LEVEL = LOGLEVEL_INFO;

// Thinking about making this static... would that make it a Singleton?
// A model interface might be nothing more than setter/getter functions
static app_model_t sAppModel = {INIT_UNINITIALIZED,
                                RUNTIME_UNINITIALIZED,
                                false,
                                false,
                                DEFAULT_APP_LOG_LEVEL};

const char *INIT_UNINITIALIZED_STR           = "INIT_UNINITIALIZED";
const char *INIT_STARTED_STR                 = "INIT_STARTED";
const char *INIT_PARSE_ARGS_COMPLETE_STR     = "INIT_PARSE_ARGS_COMPLETE";
const char *INIT_SET_ENV_COMPLETE_STR        = "INIT_SET_ENV_COMPLETE";
const char *INIT_GTK_INIT_COMPLETE_STR       = "INIT_GTK_INIT_COMPLETE";
const char *INIT_APP_BUILDER_COMPLETE_STR    = "INIT_APP_BUILDER_COMPLETE";
const char *INIT_APP_STYLING_COMPLETE_STR    = "INIT_APP_STYLING_COMPLETE";
const char *INIT_SUCCESS_STR                 = "INIT_SUCCESS";
const char *INIT_ERROR_STR                   = "INIT_ERROR";

const char *RUNTIME_UNINITIALIZED_STR        = "RUNTIME_UNINITIALIZED";
const char *RUNTIME_INITIALIZING_STR         = "RUNTIME_INITIALIZING";
const char *RUNTIME_RUNNING_NORMALLY_STR     = "RUNTIME_RUNNING_NORMALLY";
const char *RUNTIME_HANDLING_ERROR_STR       = "RUNTIME_HANDLING_ERROR";
const char *RUNTIME_RUNNING_DEPRECATED_STR   = "RUNTIME_RUNNING_DEPRECATED";
const char *RUNTIME_RUNNING_SAFE_MODE_STR    = "RUNTIME_RUNNING_SAFE_MODE";
const char *RUNTIME_SHUTTING_DOWN_STR        = "RUNTIME_SHUTTING_DOWN";

const char *get_init_state_str(APP_INIT_STATE state)
{
   if (state == INIT_UNINITIALIZED)
      return INIT_UNINITIALIZED_STR;
   else if (state == INIT_STARTED)
      return INIT_STARTED_STR;
   else if (state == INIT_PARSE_ARGS_COMPLETE)
      return INIT_PARSE_ARGS_COMPLETE_STR;
   else if (state == INIT_SET_ENV_COMPLETE)
      return INIT_SET_ENV_COMPLETE_STR;
   else if (state == INIT_GTK_INIT_COMPLETE)
      return INIT_GTK_INIT_COMPLETE_STR;
   else if (state == INIT_APP_BUILDER_COMPLETE)
      return INIT_APP_BUILDER_COMPLETE_STR;
   else if (state == INIT_APP_STYLING_COMPLETE)
      return INIT_APP_STYLING_COMPLETE_STR;
   else if (state == INIT_SUCCESS)
      return INIT_SUCCESS_STR;
   else return INIT_ERROR_STR;
}
const char *get_runtime_state_str(APP_RUNTIME_STATE state)
{
   if (state == RUNTIME_UNINITIALIZED)
      return RUNTIME_UNINITIALIZED_STR;
   else if (state == RUNTIME_INITIALIZING)
      return RUNTIME_INITIALIZING_STR;
   else if (state == RUNTIME_RUNNING_NORMALLY)
      return RUNTIME_RUNNING_NORMALLY_STR;
   else if (state == RUNTIME_HANDLING_ERROR)
      return RUNTIME_HANDLING_ERROR_STR;
   else if (state == RUNTIME_RUNNING_DEPRECATED)
      return RUNTIME_RUNNING_DEPRECATED_STR;
   else if (state == RUNTIME_RUNNING_SAFE_MODE)
      return RUNTIME_RUNNING_SAFE_MODE_STR;
   else if (state == RUNTIME_SHUTTING_DOWN)
      return RUNTIME_SHUTTING_DOWN_STR;
   else return "RUNTIME_STATE_UNKNOWN";
}

void set_app_init_state(APP_INIT_STATE state) {
   sAppModel.init_state = state;
   logging_llprintf(LOGLEVEL_INFO, "setting app model init state: %s", get_init_state_str(state));
}
APP_INIT_STATE get_app_init_state(void) {
   return sAppModel.init_state;
}

void set_app_runtime_state(APP_RUNTIME_STATE state) {
   sAppModel.runtime_state = state;
   logging_llprintf(LOGLEVEL_INFO, "setting app model runtime state");
}
APP_RUNTIME_STATE get_app_runtime_state(void) {
   return sAppModel.runtime_state;
}

void set_app_debug_flag(bool enable) {
   sAppModel.debugging_enabled = enable;
   if (enable) {
      // TODO: set and immediately use log level?
      logging_llprintf(LOGLEVEL_INFO, "debug log level enabled");
   }
}
bool get_app_debug_flag(void) {
   return sAppModel.debugging_enabled;
}

void set_app_log_w_timestamp_flag(bool enable) {
   sAppModel.log_with_timestamps = enable;
   if (enable) {
      // TODO: set and immediately use log level?
      logging_llprintf(LOGLEVEL_INFO, "logging with timestamps enabled");
   }
}
bool get_app_log_w_timestamp_flag(void) {
   return sAppModel.log_with_timestamps;
}

void set_app_log_level(LOGLEVEL logLevel) {
   if (sAppModel.runtime_log_level > logLevel) {
      sAppModel.runtime_log_level = logLevel;
      logging_llprintf(LOGLEVEL_INFO,"lowering log level from default");
   }
}
LOGLEVEL get_app_log_level(void) {
   return sAppModel.runtime_log_level;
}
