//
// Created by french on 2/3/24.
//
#include <math.h>
#include <gtk/gtk.h>
#include "comm.h"
#include "ui_builder.h"
#include "utils/logging.h"
#include "models/tictactoe.h"
#include "models/gtkdial.h"
#include "models/scalar_display.h"
#include "models/numeric_label.h"
#include "models/battery_indicator.h"

const double DIAL_INIT_VALUE = 0.0;

gboolean transform_toggle_label_on_off(GBinding *src, const GValue *fromValue,  __attribute__((unused)) GValue *toValue,
                                       __attribute__((unused)) gpointer user_data) {
    GtkToggleButton *tbutton = (GtkToggleButton *) g_binding_get_source(src);
//   GtkToggleButton *tbutton = (GtkToggleButton *) g_binding_dup_source(src);
   if (tbutton != NULL) {
      gboolean active = g_value_get_boolean(fromValue);
      gtk_button_set_label(GTK_BUTTON(tbutton), active ? "ON" : "OFF");
//     g_object_unref(tbutton);
      return TRUE;
   }
   return FALSE;
}

__attribute__((unused))
static void bind_toggle_active_label_swap(GtkToggleButton *tbutton)
{
   g_object_bind_property_full(tbutton, "active",
                               tbutton, "label",
                               G_BINDING_SYNC_CREATE,
                               (GBindingTransformFunc) transform_toggle_label_on_off,
                               NULL,NULL, NULL);
}

__attribute__((unused))
static void bind_toggle_src_active_tar_sensitive(GtkToggleButton *src, GtkWidget *tar)
{
   g_object_bind_property(src, "active", tar, "sensitive", G_BINDING_DEFAULT);
}

void genericWidgetSetup(gpointer widget, __attribute__((unused)) gpointer user_data)
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

void win( GtkWidget *widget, __attribute__((unused)) gpointer data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "You win!");
   tictactoe_clear (TICTACTOE (widget));
}

void on_component_clicked(__attribute__((unused)) GtkWidget *wdgt, __attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

void on_scalar_clicked(__attribute__((unused)) GtkWidget *wdgt, int graph_type, __attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s: signal_source = %d", __func__, graph_type);
}

app_widget_ref_struct *app_builder(void) {
   GtkBuilder              *builder;
   GtkAdjustment           *adjustment;

   app_widget_ref_struct   *appWidgetsT = g_slice_new(app_widget_ref_struct);

   builder     = gtk_builder_new();
   adjustment  = GTK_ADJUSTMENT (gtk_adjustment_new (DIAL_INIT_VALUE, 0, 100, 0.01, 0.1, 0));

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

   appWidgetsT->w_composite_anchor = GTK_WIDGET(gtk_builder_get_object(builder, "composite_anchor"));
   appWidgetsT->w_custom_anchor = GTK_WIDGET(gtk_builder_get_object(builder, "custom_anchor"));
   appWidgetsT->w_scalar_display_anchor = GTK_WIDGET(gtk_builder_get_object(builder, "scalar_display_anchor"));
   appWidgetsT->w_scalar_pressure_anchor = GTK_WIDGET(gtk_builder_get_object(builder, "scalar_pressure_anchor"));
   appWidgetsT->w_scalar_flowrate_anchor = GTK_WIDGET(gtk_builder_get_object(builder, "scalar_flowrate_anchor"));

   appWidgetsT->w_batt_indicator_anchor = GTK_WIDGET(gtk_builder_get_object(builder, "batt_indicator_anchor"));

   appWidgetsT->w_home_page_overlay = GTK_WIDGET(gtk_builder_get_object(builder, "home_page_overlay"));

   appWidgetsT->w_temp_widget_anchor = GTK_WIDGET(gtk_builder_get_object(builder, "temp_widget_anchor"));

   appWidgetsT->w_ttt = tictactoe_new();
   appWidgetsT->w_the_dial = gtk_dial_new (adjustment);
   appWidgetsT->w_dial_listener_label = numeric_label_new(DIAL_INIT_VALUE, 1);
   appWidgetsT->w_the_scalar_display = scalar_display_new("Battery", "% charge", 1);
   appWidgetsT->w_the_pressure_display = scalar_display_new("Pressure", "mmHg", 1);
   appWidgetsT->w_the_flowrate_display = scalar_display_new("NoName", NULL, 1);
   appWidgetsT->w_the_batt_indicator = battery_indicator_new();

   gtk_container_add(GTK_CONTAINER(appWidgetsT->w_composite_anchor), appWidgetsT->w_ttt);
   gtk_box_pack_start(GTK_BOX(appWidgetsT->w_custom_anchor), appWidgetsT->w_the_dial, TRUE, TRUE, 10);
   gtk_box_pack_end(GTK_BOX(appWidgetsT->w_custom_anchor), appWidgetsT->w_dial_listener_label, FALSE, TRUE, 0);
   gtk_box_pack_start(GTK_BOX(appWidgetsT->w_batt_indicator_anchor), appWidgetsT->w_the_batt_indicator, TRUE, TRUE, 10);

   gtk_box_pack_end(GTK_BOX(appWidgetsT->w_scalar_display_anchor), appWidgetsT->w_the_scalar_display, TRUE, TRUE, 10);
   gtk_box_pack_end(GTK_BOX(appWidgetsT->w_scalar_pressure_anchor), appWidgetsT->w_the_pressure_display, TRUE, TRUE, 10);
   gtk_box_pack_end(GTK_BOX(appWidgetsT->w_scalar_flowrate_anchor), appWidgetsT->w_the_flowrate_display, TRUE, TRUE, 10);

   // Gross... we
   g_object_bind_property(appWidgetsT->w_the_dial, "old_value", appWidgetsT->w_dial_listener_label, "value", G_BINDING_DEFAULT);
   g_object_bind_property(appWidgetsT->w_dial_listener_label, "value", appWidgetsT->w_the_scalar_display, "value", G_BINDING_DEFAULT);
   g_object_bind_property(appWidgetsT->w_dial_listener_label, "value", appWidgetsT->w_the_batt_indicator, "value", G_BINDING_DEFAULT);

   appWidgetsT->w_temp_composite = NULL;

   // ********************************************************************************
   // signals and bindings
   // ********************************************************************************
   g_signal_connect (G_OBJECT(appWidgetsT->w_the_scalar_display), "scalar-display-pressed",
                     G_CALLBACK(on_scalar_clicked), appWidgetsT);

   gtk_builder_connect_signals(builder, appWidgetsT);

   g_signal_connect (G_OBJECT(appWidgetsT->w_ttt), "tictactoe", G_CALLBACK(win), NULL);



   g_signal_connect (G_OBJECT(appWidgetsT->w_the_batt_indicator), "button-press-event",
                     G_CALLBACK(on_component_clicked), appWidgetsT);

   // It appears the composite/custom widgets with templates are loaded and show, where these
   // seem to need explicit exposure.
   gtk_widget_show(appWidgetsT->w_the_dial);
   gtk_widget_show(appWidgetsT->w_the_batt_indicator);
   gtk_widget_show(appWidgetsT->w_ttt);

   scalar_display_set_value(SCALAR_DISPLAY(appWidgetsT->w_the_scalar_display), DIAL_INIT_VALUE);

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
