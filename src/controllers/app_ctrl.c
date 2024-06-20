//
// Created by french on 2/4/24.
//
#include "app_ctrl.h"
#include "comm.h"
#include "utils/sys_interface.h"
#include "utils/parse_args.h"
#include "utils/logging.h"
#include "views/ui_builder.h"
#include "views/css_styler.h"
#include "models/app_model.h"

app_widget_ref_struct *g_app_widget_refs;

void app_init(int argc, char **argv) {
   RVALUE retVal;

   set_app_init_state(INIT_STARTED);

   retVal = parse_input_args(argc, argv);
   if (retVal == PARSE_ARG_ERR) {
      set_app_init_state(INIT_ERROR);
      return;
   }

   set_app_init_state(INIT_PARSE_ARGS_COMPLETE);

   retVal = set_display_env();
   if (retVal == DISPLAY_ENV_ERR) {
      set_app_init_state(INIT_ERROR);
      return;
   }
   set_app_init_state(INIT_SET_ENV_COMPLETE);

   gtk_init(&argc, &argv);
   set_app_init_state(INIT_GTK_INIT_COMPLETE);

   g_app_widget_refs = app_builder();
   set_app_init_state(INIT_APP_BUILDER_COMPLETE);

   apply_app_styling(g_app_widget_refs);
   set_app_init_state(INIT_APP_STYLING_COMPLETE);

   set_app_init_state(INIT_SUCCESS);
}

void app_finalize(void)
{
   logging_llprintf(LOGLEVEL_INFO, "Goodbye!");
}
