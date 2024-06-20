//
// Created by french on 2/3/24.
//
#include <gtk/gtk.h>
#include "comm.h"
#include "ui_builder.h"
#include "utils/logging.h"

gboolean transform_toggle_label_on_off(GBinding *src, const GValue *fromValue,  __attribute__((unused)) GValue *toValue,
                                       __attribute__((unused)) gpointer user_data) {
   GtkToggleButton *tbutton = (GtkToggleButton *) g_binding_get_source(src);
   if (tbutton != NULL) {
      gboolean active = g_value_get_boolean(fromValue);
      gtk_button_set_label(GTK_BUTTON(tbutton), active ? "ON" : "OFF");
//      g_object_unref(tbutton);
      return TRUE;
   }
   return FALSE;
}

static void bind_toggle_active_label_swap(GtkToggleButton *tbutton)
{
   g_object_bind_property_full(tbutton, "active",
                               tbutton, "label",
                               G_BINDING_SYNC_CREATE,
                               (GBindingTransformFunc) transform_toggle_label_on_off,
                               NULL,NULL, NULL);
}

static void bind_toggle_src_active_tar_sensitive(GtkToggleButton *src, GtkWidget *tar)
{
   g_object_bind_property(src, "active", tar, "sensitive", G_BINDING_DEFAULT);
}

void genericWidgetSetup(gpointer widget, gpointer user_data)
{
//   app_widget_ref_struct *app_wdgts = (app_widget_ref_struct*) user_data;
   if (GTK_IS_WIDGET(widget)){
//      const char* wName = gtk_widget_get_name(GTK_WIDGET(widget));
      if (GTK_IS_TOGGLE_BUTTON(widget) && !GTK_IS_RADIO_BUTTON(widget) && !GTK_IS_CHECK_BUTTON(widget))
      {
         gtk_style_context_add_class(gtk_widget_get_style_context(widget), "toggle-button");
      }
   }
}

app_widget_ref_struct *app_builder(void) {
   GtkBuilder *builder;

   app_widget_ref_struct *appWidgetsT = g_slice_new(app_widget_ref_struct);

   builder = gtk_builder_new();

   if (gtk_builder_add_from_resource(builder, "/mini_app/resources/mini_app.glade", NULL) == 0) {
      logging_llprintf(LOGLEVEL_ERROR, "failed to load glade resource %s", "mini_app.glade");
      return NULL;
   }

   // ********************************************************************************
   // Map the objects we've described in the glade file to our widget references
   // ********************************************************************************
   appWidgetsT->w_min_main_wnd = GTK_WIDGET(gtk_builder_get_object(builder, "min_main_wnd"));
   appWidgetsT->w_min_close = GTK_WIDGET(gtk_builder_get_object(builder, "min_close"));
   appWidgetsT->w_msg_out_textview = GTK_WIDGET(gtk_builder_get_object(builder, "msg_out_textview"));

   appWidgetsT->w_do_something_button = GTK_WIDGET(gtk_builder_get_object(builder, "do_something_button"));
   appWidgetsT->w_say_something_entry = GTK_WIDGET(gtk_builder_get_object(builder, "say_something_entry"));

   // ********************************************************************************
   /* appWidgetsT->w_glade_ID = GTK_WIDGET(gtk_builder_get_object(builder, "glade_ID")); */

   // ********************************************************************************
   // signals and bindings
   // ********************************************************************************
   gtk_builder_connect_signals(builder, appWidgetsT);

   // Call the generic widget setup function on all the widgets
   // This lets us easily set up signal handlers for certain classes of widget (e.g. block scrolling, show number pad)
   GSList *list = gtk_builder_get_objects(builder);
   g_slist_foreach(list, genericWidgetSetup, appWidgetsT);
   g_slist_free(list);
   // ********************************************************************************
   // Release anything we've instantiated with a ref-count or anything we've received allocated copies of
   // ********************************************************************************
   g_object_unref(builder);
   return appWidgetsT;
}
