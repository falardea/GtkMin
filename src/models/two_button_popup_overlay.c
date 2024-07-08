/*
 * Copyright (c) 2024 Deka Research and Development. All rights reserved.
 * Created by rfalardeau on 5/30/2024. 
*/
#include <gtk/gtk.h>
#include "comm.h"
#include "two_button_popup_overlay.h"

#define BLOCK_PLACEHOLDER_TEXT "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt"\
" ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip"\
" ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur."

GtkWidget *popup_box = NULL;
static UserPromptResponseCallback_T on_user_accept;
static UserPromptResponseCallback_T on_user_reject;

OVERLAY_POPUP_PROPERTIES_T popup_flavors[NUM_OVERLAY_POPUP_FLAVORS] = {
      {
            "Yes/No Title", "Do you wish to continue?", "YES", "NO"
      },
      {
            "Start/Cancel Title", "We're ready to run", "Start", "Cancel"
      },
      {
            "Confirm/Cancel Title", "Confirm or cancel to dismiss", "Confirm", "Cancel"
      }
};

void on_popup_accept_btn_clicked(__attribute__((unused)) GtkButton *button, app_widget_ref_struct *app_wdgts)
{
   g_print("<<DEBUG-%s>> overlay button pressed! \n",__func__);
   on_user_accept(app_wdgts);
   gtk_widget_destroy(popup_box);
   popup_box = NULL;
}

void on_popup_reject_btn_clicked(__attribute__((unused)) GtkButton *button, app_widget_ref_struct *app_wdgts)
{
   g_print("<<DEBUG-%s>> overlay button pressed! \n",__func__);
   on_user_reject(app_wdgts);
   gtk_widget_destroy(popup_box);
   popup_box = NULL;
}

void build_and_show_two_button_prompt(app_widget_ref_struct *app_wdgts,
                                      UserPromptResponseCallback_T user_accept,
                                      UserPromptResponseCallback_T user_reject,
                                      GtkOverlay *destination,
                                      OVERLAY_POPUP_FLAVORS dialog_flavoring)
{
   g_print("<<DEBUG-%s>> popup?!\n", __func__);
   on_user_accept = user_accept;
   on_user_reject = user_reject;

   if (popup_box != NULL)
   {
      g_print("popup box !NULL");
      gtk_widget_destroy(popup_box);
   }
   popup_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
   gtk_widget_set_name(popup_box, "modal-container-for-overlay");

   GtkWidget *popup_box_drawing_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
   gtk_widget_set_name(popup_box_drawing_area, "overlay-popup-box");

   GtkWidget *popup_title_n_text = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
//   GtkWidget *popup_title_lbl = gtk_label_new("User Input Required or Requested");
   GtkWidget *popup_title_lbl = gtk_label_new(popup_flavors[dialog_flavoring].title);

   GtkWidget *popup_user_msg_lbl = gtk_label_new(popup_flavors[dialog_flavoring].user_msg);

   GtkWidget *popup_user_input_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
   GtkWidget *popup_accept_btn = gtk_button_new_with_label(popup_flavors[dialog_flavoring].accept_btn_text);
   GtkWidget *popup_reject_btn = gtk_button_new_with_label(popup_flavors[dialog_flavoring].reject_btn_text);

   gtk_box_pack_start (GTK_BOX(popup_box), popup_box_drawing_area, TRUE, TRUE, 0);
   gtk_box_pack_start (GTK_BOX(popup_box_drawing_area), popup_title_n_text, TRUE, TRUE, 10);
   gtk_box_pack_start (GTK_BOX(popup_box_drawing_area), popup_user_input_box, TRUE, TRUE, 10);

   gtk_box_pack_start (GTK_BOX(popup_title_n_text), popup_title_lbl, FALSE, TRUE, 0);
   gtk_box_pack_start (GTK_BOX(popup_title_n_text), popup_user_msg_lbl, FALSE, TRUE, 0);

   // Title and text centering, word wrapping and max char width
   gtk_label_set_justify(GTK_LABEL(popup_title_lbl), GTK_JUSTIFY_CENTER);
   gtk_label_set_justify(GTK_LABEL(popup_user_msg_lbl), GTK_JUSTIFY_CENTER);
   gtk_label_set_line_wrap(GTK_LABEL(popup_title_lbl), TRUE);
   gtk_label_set_line_wrap(GTK_LABEL(popup_user_msg_lbl), TRUE);
   gtk_label_set_line_wrap_mode(GTK_LABEL(popup_title_lbl), PANGO_WRAP_WORD);
   gtk_label_set_line_wrap_mode(GTK_LABEL(popup_user_msg_lbl), PANGO_WRAP_WORD);
   gtk_label_set_width_chars(GTK_LABEL(popup_title_lbl), 60);
   gtk_label_set_width_chars(GTK_LABEL(popup_user_msg_lbl), 60);
   gtk_label_set_max_width_chars(GTK_LABEL(popup_title_lbl), 80);
   gtk_label_set_max_width_chars(GTK_LABEL(popup_user_msg_lbl), 80);

   gtk_box_pack_start (GTK_BOX(popup_user_input_box), popup_accept_btn, FALSE, TRUE, 0);
   gtk_box_pack_start (GTK_BOX(popup_user_input_box), popup_reject_btn, FALSE, TRUE, 0);

   gtk_button_box_set_layout(GTK_BUTTON_BOX(popup_user_input_box), GTK_BUTTONBOX_SPREAD);

   gtk_widget_set_halign(popup_box, GTK_ALIGN_FILL);
   gtk_widget_set_valign(popup_box, GTK_ALIGN_FILL);

   gtk_widget_set_halign(popup_box_drawing_area, GTK_ALIGN_CENTER);
   gtk_widget_set_valign(popup_box_drawing_area, GTK_ALIGN_CENTER);

   gtk_widget_set_margin_top(popup_title_n_text, 10);
   gtk_widget_set_margin_start(popup_title_n_text, 30);
   gtk_widget_set_margin_end(popup_title_n_text, 30);
   gtk_widget_set_margin_start(popup_user_input_box, 30);
   gtk_widget_set_margin_end(popup_user_input_box, 30);
   gtk_widget_set_margin_bottom(popup_user_input_box, 10);

   gtk_overlay_add_overlay(destination, popup_box);

   g_signal_connect(GTK_BUTTON(popup_accept_btn), "clicked", G_CALLBACK(on_popup_accept_btn_clicked), app_wdgts);
   g_signal_connect(GTK_BUTTON(popup_reject_btn), "clicked", G_CALLBACK(on_popup_reject_btn_clicked), app_wdgts);

   gtk_widget_show_all(popup_box);
}
