//
// Created by french on 2/3/24.
//

#ifndef MINI_APP_APP_VIEW_H
#define MINI_APP_APP_VIEW_H
#ifdef __cplusplus
extern "C" {
#endif

#include <gtk/gtk.h>

void on_min_close_clicked(__attribute__((unused)) GtkWidget *srcWidget,
                                __attribute__((unused)) gpointer uData);

gboolean on_min_main_wnd_delete_event(__attribute__((unused)) GtkWidget *srcWidget,
                                    __attribute__((unused)) GdkEvent *event,
                                    __attribute__((unused)) gpointer uData);

#endif  // MINI_APP_APP_VIEW_H
