/*
 * Copyright (c) 2024 Deka Research and Development. All rights reserved.
 * Created by rfalardeau on 5/30/2024. 
*/

#ifndef TWO_BUTTON_POPUP_OVERLAY_H__
#define TWO_BUTTON_POPUP_OVERLAY_H__
#include "comm.h"

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

#endif  // TWO_BUTTON_POPUP_OVERLAY_H__
