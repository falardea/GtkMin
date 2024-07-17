/*
 * Copyright (c) 2024
 * Created by rfalardeau on 7/1/2024. 
*/
#ifndef GTKMIN_SCALAR_DISPLAY_H_
#define GTKMIN_SCALAR_DISPLAY_H_
#include <gtk/gtk.h>
#include "comm.h"

G_BEGIN_DECLS // C++ guard, from gmacros.h

 #define SCALAR_TYPE_DISPLAY              ( scalar_display_get_type( ) )
 G_DECLARE_FINAL_TYPE(ScalarDisplay, scalar_display, SCALAR, DISPLAY, GtkBox)

GtkWidget*           scalar_display_new               (const char* scalar_name,
                                                       const char* scalar_units,
                                                       guint scalar_precision);

gchar                *scalar_display_get_name_str     (ScalarDisplay *self);
void                 scalar_display_set_name_str      (ScalarDisplay *self, const char* name);

gchar                *scalar_display_get_units_str    (ScalarDisplay *self);
void                 scalar_display_set_units_str     (ScalarDisplay *self, const char* units);

gdouble              scalar_display_get_value         (ScalarDisplay *self);
void                 scalar_display_set_value         (ScalarDisplay *self, gdouble new_value);

guint                scalar_display_get_precision     (ScalarDisplay *self);
void                 scalar_display_set_precision     (ScalarDisplay *self, guint scalar_precision);

gboolean             scalar_display_get_uncalibrated  (ScalarDisplay *self);
void                 scalar_display_set_uncalibrated  (ScalarDisplay *self, gboolean is_uncalibrated);

gboolean             scalar_display_get_has_error     (ScalarDisplay *self);
void                 scalar_display_set_has_error     (ScalarDisplay *self, gboolean errored);

gboolean             scalar_display_get_value_oor     (ScalarDisplay *self);
void                 scalar_display_set_value_oor     (ScalarDisplay *self, gboolean is_oor);

G_END_DECLS
#endif  // GTKMIN_SCALAR_DISPLAY_H_
