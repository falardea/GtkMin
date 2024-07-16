/*
 * Created by french on 2/18/24.
*/
#ifndef MINI_APP_COMMON_DEFS_H
#define MINI_APP_COMMON_DEFS_H
#include <gtk/gtk.h>
#include <glib-object.h>

G_BEGIN_DECLS // C++ guard, from gmacros.h

#define REALLY_LONG_ISO_TIME_STRING "2000-01-31T12:34:56.789-06:00"
#define DEFAULT_TIMESTAMP_LOG_FORMAT "%04d-%02d-%02dT%02d:%02d:%02d"

typedef enum {
   SUCCESS,
   PARSE_ARG_ERR,
   DISPLAY_ENV_ERR // TODO: fill in more values here... or delete this
} RVALUE;

typedef enum {
   LOGLEVEL_DEBUG,
   LOGLEVEL_INFO,
   LOGLEVEL_ERROR
} LOGLEVEL;

typedef enum
{
   SCALAR_DISPLAY_ARTERIAL_OXYGEN,
   SCALAR_DISPLAY_VENOUS_OXYGEN,
   SCALAR_DISPLAY_ARTERIAL_PRESSURE,
   N_SCALAR_DISPLAY_MEASUREMENT_TYPES
} SCALAR_DISPLAY_MEASUREMENT_TYPES;

extern const char* DEBUG_STR;
extern const char* INFO_STR;
extern const char* ERROR_STR;
extern const int   LOGGING_MAX_MSG_LENGTH;

typedef struct {
   // ********************************************************************************
   // Basic app framework
   GtkWidget   *w_min_main_wnd;
   GtkWidget   *w_msg_out_textview;
   GtkWidget   *w_min_close;
   GtkWidget   *w_do_something_button;
   GtkWidget   *w_say_something_entry;

   GtkWidget   *w_composite_anchor;
   GtkWidget   *w_custom_anchor;
   GtkWidget   *w_scalar_display_anchor;

   GtkWidget   *w_ttt;
   GtkWidget   *w_the_dial;
   GtkWidget   *w_dial_listener_label;

   GtkWidget   *w_the_scalar_display;

   GtkWidget   *w_batt_indicator_anchor;
   GtkWidget   *w_the_batt_indicator;

   GtkWidget   *w_home_page_overlay;
//   GtkWidget   *w_two_button_popup_root;

   GtkWidget   *w_temp_widget_anchor;
   GtkWidget   *w_temp_composite;
   /*
    GtkWidget   *w_
    */

} app_widget_ref_struct;

extern app_widget_ref_struct *g_app_widget_refs;

G_END_DECLS
#endif  /* MINI_APP_COMMON_DEFS_H */
