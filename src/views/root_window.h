//
// Created by french on 2/3/24.
//

#ifndef MINI_APP_APP_VIEW_H
#define MINI_APP_APP_VIEW_H
#include <gtk/gtk.h>

G_BEGIN_DECLS // C++ guard, from gmacros.h

void on_min_close_clicked(__attribute__((unused)) GtkWidget *srcWidget,
                                __attribute__((unused)) gpointer uData);

gboolean on_min_main_wnd_delete_event(__attribute__((unused)) GtkWidget *srcWidget,
                                    __attribute__((unused)) GdkEvent *event,
                                    __attribute__((unused)) gpointer uData);

G_END_DECLS
#endif  // MINI_APP_APP_VIEW_H
