/*
 * Created by french on 6/19/24.
*/
#include <gtk/gtk.h>
#include "root_child_msgout.h"
#include "models/tictactoe.h"
#include "models/gtkdial.h"


void win( GtkWidget *widget, __attribute__((unused)) gpointer data)
{
   print_log_level_msgout(LOGLEVEL_DEBUG, "Yay!\n");
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
         print_log_level_msgout(LOGLEVEL_DEBUG, "add");
         ttt = tictactoe_new();
         gtk_container_add(GTK_CONTAINER(wdgts->w_composite_anchor), ttt);
         gtk_widget_show(ttt);
         g_signal_connect (G_OBJECT(ttt), "tictactoe", G_CALLBACK(win), NULL);
         shown = TRUE;
      }
      else
      {
         print_log_level_msgout(LOGLEVEL_DEBUG, "remove?");
         gtk_container_foreach(GTK_CONTAINER(wdgts->w_composite_anchor),
                               wipe_children,
                               GTK_CONTAINER(wdgts->w_composite_anchor));
         shown = FALSE;
      }
   }
}

void on_do_something_else_button_clicked(GtkButton *button, gpointer user_data)
{
   GtkWidget      *dial;
   GtkAdjustment  *adjustment;

   app_widget_ref_struct *wdgts = (app_widget_ref_struct *) user_data;

   adjustment = GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 100, 0.01, 0.1, 0));
//   gtk_widget_set_size_request(wdgts->w_custom_anchor, 400, 200);
   dial = gtk_dial_new (adjustment);

   gtk_box_pack_start(GTK_BOX(wdgts->w_custom_anchor), dial, TRUE, TRUE, 10);

//   gtk_container_add(GTK_CONTAINER(wdgts->w_custom_anchor), dial);
   gtk_widget_show(dial);
}