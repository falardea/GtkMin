/*
 * Created by french on 2/17/24.
*/
#include <stdio.h>
#include <string.h>
#include "../comm.h"
#include "../models/app_model.h"
#include "parse_args.h"

static void print_usage(char *argv) {
   const char *exeName = strrchr(argv, '/') + 1;  // the +1 removes the leading '/'
   printf("Usage: %s\n"
          "A mini_app project for experimenting with GTK3 and design patterns in C.\n"
          " -D --debug\tEnable Debugging and enable Debug (full) level logging\n"
          " -q --quiet\tRaise log level to ERROR\n"
          " +t --timestamps\tEnable timestamps\n"
          " -h --help\tPrint this help message\n\n", exeName);
}

RVALUE parse_input_args(int argc, char **argv) {
   for (int i = 1; i < argc; i++) {
      if ((strcmp(argv[i], "-D") == 0) || (strcmp(argv[i], "--debug") == 0))
      {
         set_app_debug_flag(true);
         set_app_log_level(LOGLEVEL_DEBUG);
      } else if ((strcmp(argv[i], "-q") == 0) || (strcmp(argv[i], "--quiet") == 0))
      {
         set_app_log_level(LOGLEVEL_ERROR);
      } else if ((strcmp(argv[i], "+t") == 0) || (strcmp(argv[i], "--timestamps") == 0))
      {
         set_app_log_w_timestamp_flag(true);
      } else if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0))
      {
         // TODO: Is the only difference here the explicit call for help?  Maybe not an error, just excuse to exit?
         print_usage(argv[0]);
      } else if (strlen(argv[i]) > 0) {
         print_usage(argv[0]);
         return PARSE_ARG_ERR;
      }
   }
   return SUCCESS;
}
