/*
 * Created by french on 6/19/24.
*/
#include <gtk/gtk.h>
#include "comm.h"
#include "utils/logging.h"
#include "views/root_child_msgout.h"
#include "models/two_button_popup_overlay.h"
#include "models/temp_composite.h"

static const gchar *label_str = "FOOBITTY";

void wipe_children(GtkWidget *wdgt, gpointer udata)
{
   gtk_container_remove(GTK_CONTAINER(udata), wdgt);
}

void on_do_something_button_clicked(__attribute__((unused)) GtkButton *button, __attribute__((unused)) gpointer user_data)
{
   static gboolean shown = FALSE;

   app_widget_ref_struct *wdgts = (app_widget_ref_struct *) user_data;
   if (gtk_entry_buffer_get_length(gtk_entry_get_buffer(GTK_ENTRY(wdgts->w_say_something_entry))) > 0){
      print_log_level_msgout(LOGLEVEL_DEBUG, "%s", gtk_entry_get_text(GTK_ENTRY(wdgts->w_say_something_entry)));
   } else {
      print_log_level_msgout(LOGLEVEL_DEBUG, "boink!");
   }

   wdgts->w_two_button_popup_root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
   GtkWidget *popup = two_button_popup_new(wdgts->w_two_button_popup_root);
   gtk_box_pack_start (GTK_BOX(wdgts->w_two_button_popup_root), popup, TRUE, TRUE, 0);

   gtk_overlay_add_overlay(GTK_OVERLAY(wdgts->w_home_page_overlay), wdgts->w_two_button_popup_root);
   gtk_widget_show_all(wdgts->w_two_button_popup_root);
}


void on_label_clicked(__attribute__((unused)) GtkWidget *wdgt, __attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

void on_do_something_else_button_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   app_widget_ref_struct *wdgts = (app_widget_ref_struct *) user_data;
   logging_llprintf(LOGLEVEL_DEBUG, "%s: CHECKPOINT", __func__);

   if (wdgts->w_temp_composite != NULL)
   {
      gtk_widget_destroy(wdgts->w_temp_composite);
      wdgts->w_temp_composite = NULL;
   }
   wdgts->w_temp_composite = temp_composite_new(label_str);
   gtk_box_pack_end(GTK_BOX(wdgts->w_temp_widget_anchor), wdgts->w_temp_composite, FALSE, TRUE, 0);

   gtk_widget_show(wdgts->w_temp_composite);

   g_signal_connect (G_OBJECT(wdgts->w_the_scalar_display),
                     "button-press-event",
                     G_CALLBACK(on_label_clicked),
                     wdgts);
}