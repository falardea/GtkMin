/*
 * Created by french on 6/19/24.
*/
#include <gtk/gtk.h>
#include "comm.h"
#include "models/tictactoe.h"
#include "models/gtkdial.h"
#include "models/scalar_display.h"
#include "views/root_child_msgout.h"

void win( GtkWidget *widget, __attribute__((unused)) gpointer data)
{
   print_log_level_msgout(LOGLEVEL_DEBUG, "You win!");
   tictactoe_clear (TICTACTOE (widget));
}

void wipe_children(GtkWidget *wdgt, gpointer udata)
{
   gtk_container_remove(GTK_CONTAINER(udata), wdgt);
}

void on_do_something_button_clicked(__attribute__((unused)) GtkButton *button, __attribute__((unused)) gpointer user_data)
{
   static gboolean shown = FALSE;
   GtkWidget *ttt;

   app_widget_ref_struct *wdgts = (app_widget_ref_struct *) user_data;
   if (gtk_entry_buffer_get_length(gtk_entry_get_buffer(GTK_ENTRY(wdgts->w_say_something_entry))) > 0){
      print_log_level_msgout(LOGLEVEL_DEBUG, "%s", gtk_entry_get_text(GTK_ENTRY(wdgts->w_say_something_entry)));
   } else {
      if (!shown){
         print_log_level_msgout(LOGLEVEL_DEBUG, "added tictactoe");
         ttt = tictactoe_new();
         gtk_container_add(GTK_CONTAINER(wdgts->w_composite_anchor), ttt);
         gtk_widget_show(ttt);
         g_signal_connect (G_OBJECT(ttt), "tictactoe", G_CALLBACK(win), NULL);
         shown = TRUE;
      }
      else
      {
         print_log_level_msgout(LOGLEVEL_DEBUG, "removed tictactoe");
         gtk_container_foreach(GTK_CONTAINER(wdgts->w_composite_anchor),
                               wipe_children,
                               GTK_CONTAINER(wdgts->w_composite_anchor));
         shown = FALSE;
      }
   }
}

void on_dial_change(GtkWidget *wdgt, double value, gpointer user_data)
{
   app_widget_ref_struct *wdgts = (app_widget_ref_struct *) user_data;

   char dial_value_str[32];
   snprintf(dial_value_str, sizeof (dial_value_str), "%3.1f", value);
   gtk_label_set_label(GTK_LABEL(wdgts->w_dial_listener_label), dial_value_str);
}

void on_do_something_else_button_clicked(GtkButton *button, gpointer user_data)
{
   GtkAdjustment  *adjustment;

   app_widget_ref_struct *wdgts = (app_widget_ref_struct *) user_data;

   if (wdgts->w_the_dial != NULL)
   {
      print_log_level_msgout(LOGLEVEL_DEBUG, "destroying old dial");
      gtk_widget_destroy(wdgts->w_the_dial);
   }

   if (wdgts->w_the_scalar_display != NULL)
   {
      print_log_level_msgout(LOGLEVEL_DEBUG, "destroying old scalar display");
      gtk_widget_destroy(wdgts->w_scalar_display_anchor);
   }

   adjustment = GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 100, 0.01, 0.1, 0));

   print_log_level_msgout(LOGLEVEL_DEBUG, "building new dial");
   wdgts->w_the_dial = gtk_dial_new (adjustment);

   print_log_level_msgout(LOGLEVEL_DEBUG, "building new scalar display");
   wdgts->w_the_scalar_display = scalar_display_new("pressure", "psi", "%2.1f", 0.0, 100.0);

   gtk_box_pack_start(GTK_BOX(wdgts->w_custom_anchor), wdgts->w_the_dial, TRUE, TRUE, 10);
   gtk_box_pack_end(GTK_BOX(wdgts->w_scalar_display_anchor), wdgts->w_the_scalar_display, TRUE, TRUE, 10);

   gtk_widget_show(wdgts->w_dial_listener_label);
   gtk_widget_show(wdgts->w_the_dial);
   gtk_widget_show(wdgts->w_the_scalar_display);
   gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(wdgts->w_the_scalar_display)), "scalar_display");

   g_signal_connect (G_OBJECT(wdgts->w_the_dial),
                     "dial-changed",
                     G_CALLBACK(on_dial_change),
                     wdgts);
}