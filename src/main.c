/*
 * Created by french sometime in early 2024.
*/
#include <gtk/gtk.h>
#include "version.h"
#include "models/app_model.h"
#include "controllers/app_ctrl.h"
#include "utils/logging.h"

int main(int argc, char **argv) {
   printf("ver.%d.%d.%d\n", project_VERSION_MAJOR, project_VERSION_MINOR, project_VERSION_PATCH);

   app_init(argc, argv);

   if (get_app_init_state() != INIT_ERROR)
   {
      logging_llprintf(LOGLEVEL_INFO, "application initialized, starting UI");
      gtk_main();
      logging_llprintf(LOGLEVEL_INFO, "UI closed, shutting down");
   }
   else
   {
      set_app_runtime_state(RUNTIME_HANDLING_ERROR);
      logging_llprintf(LOGLEVEL_ERROR, "application initialization failed, attempting a graceful shutdown");
      // Graceful shutdown stuff here
      set_app_runtime_state(RUNTIME_SHUTTING_DOWN);
   }

   app_finalize();
   return get_app_runtime_state();;
}
