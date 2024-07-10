/*
 * Copyright (c) 2024 Deka Research and Development. All rights reserved.
 * Created by rfalardeau on 5/30/2024. 
*/
#ifndef TWO_BUTTON_POPUP_OVERLAY_H__
#define TWO_BUTTON_POPUP_OVERLAY_H__
#include "comm.h"
G_BEGIN_DECLS

typedef struct
{
   const char  *title;
   const char  *user_msg;
   const char  *accept_btn_text;
   const char  *reject_btn_text;
} OVERLAY_POPUP_PROPERTIES_T;

typedef enum
{
   YES_NO_PROMPT,
   START_CANCEL,
   CONFIRM_CANCEL,
   NUM_OVERLAY_POPUP_FLAVORS
} OVERLAY_POPUP_FLAVORS;

typedef void (*UserPromptResponseCallback_T)(app_widget_ref_struct *wdgts);

void build_and_show_two_button_prompt(app_widget_ref_struct *app_wdgts,
                                      UserPromptResponseCallback_T user_accept,
                                      UserPromptResponseCallback_T user_reject,
                                      GtkOverlay *destination,
                                      OVERLAY_POPUP_FLAVORS dialog_flavoring);

void temp_popup_launcher(app_widget_ref_struct *wdgts);


//// New Two Button

#define TWO_BUTTON_TYPE_POPUP             ( two_button_popup_get_type () )
G_DECLARE_FINAL_TYPE(TwoButtonPopup, two_button_popup, two_button, popup, GtkBox )
struct _TwoButtonPopup
{
   GtkBox         parent;
   GtkLabel       *title_label;
   GtkLabel       *message_label;
   GtkButton      *btn_accept;
   GtkButton      *btn_reject;
   void           (* on_btn_accept_clicked) (GtkButton *button, app_widget_ref_struct *app_wdgts);
   void           (* on_btn_reject_clicked) (GtkButton *button, app_widget_ref_struct *app_wdgts);
};
GtkWidget*        two_button_popup_new       (GtkWidget *destination);
GType             two_button_popup_get_type  ();

G_END_DECLS
#endif  // TWO_BUTTON_POPUP_OVERLAY_H__
