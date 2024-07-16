/*
 * Copyright (c) 2024 Deka Research and Development. All rights reserved.
 * Created by rfalardeau on 5/30/2024. 
*/
#ifndef TWO_BUTTON_POPUP_OVERLAY_H__
#define TWO_BUTTON_POPUP_OVERLAY_H__
#include "comm.h"
G_BEGIN_DECLS

#define TWO_BUTTON_TYPE_POPUP             ( two_button_popup_get_type () )
G_DECLARE_FINAL_TYPE(TwoButtonPopup, two_button_popup, TWO_BUTTON, POPUP, GtkBox )

GtkWidget*        two_button_popup_new       ();
GType             two_button_popup_get_type  ();

G_END_DECLS
#endif  // TWO_BUTTON_POPUP_OVERLAY_H__
