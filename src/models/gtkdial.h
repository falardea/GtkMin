/*
 * Copyright (c) 2024 Deka Research and Development. All rights reserved.
 * Created by rfalardeau on 6/25/2024.
 *
 * This file is an implementation of the GtkDial from the following tutorial:
 * https://www.geany.org/manual/gtk/gtk-tutorial/x2312.html
 *
*/
#ifndef GTKMIN_GTKDIAL_H_
#define GTKMIN_GTKDIAL_H_
#include <gdk/gdk.h>
#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GTK_DIAL(obj)               G_TYPE_CHECK_INSTANCE_CAST(obj, gtk_dial_get_type(), GtkDial)
#define GTK_DIAL_CLASS(klass)       G_TYPE_CHECK_CLASS_CAST(klass, gtk_dial_get_type(), GtkDialClass)
#define GTK_IS_DIAL(obj)            G_TYPE_CHECK_INSTANCE_TYPE(obj, gtk_dial_get_type())

typedef struct _GtkDial          GtkDial;
typedef struct _GtkDialClass      GtkDialClass;

struct _GtkDial
{
   GtkWidget   widget;

   /* update policy (GTK_UPDATE_[CONTINUOUS|DELAYED|DISCONTINUOUS]) */
   guint       policy : 2;

   /* Button currently pressed or 0 in not pressed */
   guint8      button;

   /* Dial 'physical' dimensions */
   gint        radius;
   gint        pointer_width;

   /* Update timer ID */
   guint32     ud_timer;

   /* Current angular position of dial */
   gdouble     curr_angle;
   gdouble     last_angle;

   /* "Old values from adjustment stored so we know when something changes" - tutorial */
   gdouble     old_value;
   gdouble     old_lower;
   gdouble     old_upper;

   /* The adjustment object that stores the data for this dial */
   GtkAdjustment *adjustment;
};

struct _GtkDialClass
{
   GtkWidgetClass parent_class;
};

GtkWidget*        gtk_dial_new               (GtkAdjustment *adjustment);
GType             gtk_dial_get_type          (void);
GtkAdjustment*    gtk_dial_get_adjustment    (GtkDial *dial);
void              gtk_dial_set_update_policy (GtkDial *dial,
                                              guint8 policy);  // GtkUpdateType?

void              gtk_dial_set_adjustment    (GtkDial *dial,
                                              GtkAdjustment *adjustment);

#ifdef __cplusplus
}  /* closing brace for extern "C" */
#endif
#endif  // GTKMIN_GTKDIAL_H_
