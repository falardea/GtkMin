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

//#define SCALAR_TYPE_DISPLAY            ( scalar_display_get_type () )
//#define SCALAR_DISPLAY(obj)            ( G_TYPE_CHECK_INSTANCE_CAST ( (obj), scalar_display_get_type (), ScalarDisplay ))
//#define SCALAR_DISPLAY_CLASS(klass)    ( G_TYPE_CHECK_CLASS_CAST ( (klass) , scalar_display_get_type (), ScalarDisplayClass ))
//#define SCALAR_IS_DISPLAY(obj)         ( G_TYPE_CHECK_INSTANCE_TYPE ( (obj) , scalar_display_get_type () ))
//#define SCALAR_IS_DISPLAY_CLASS(klass) ( G_TYPE_CHECK_CLASS_TYPE ( (klass) , SCALAR_TYPE_DISPLAY ))
//#define SCALAR_DISPLAY_GET_CLASS(obj)  ( G_TYPE_INSTANCE_GET_CLASS ( (obj), SCALAR_TYPE_DISPLAY, ScalarDisplayClass ))

//typedef struct _ScalarDisplay          ScalarDisplay;
//typedef struct _ScalarDisplayClass     ScalarDisplayClass;

struct _ScalarDisplay
{
   GtkBox         parent;
   GtkLabel       *name_label;
   GtkLabel       *value_label;
   GtkLabel       *units_label;

   gchar          *name_str;
   gchar          *units_str;
   gchar          *format_str;

   gdouble        value;

   gboolean       has_error;
   gboolean       value_oor;
   gboolean       uncalibrated;
   // void           (* scalar_display_button_press) (GtkButton *button, app_widget_ref_struct *app_wdgts);
};

//struct _ScalarDisplayClass
//{
//   GtkBoxClass    parent_class;
//};

GtkWidget*           scalar_display_new               (const char* scalar_name,
                                                       const char* scalar_units,
                                                       const char* format_str);
//GType                scalar_display_get_type          ();

gchar                *scalar_display_get_name_str     (ScalarDisplay *self);
void                 scalar_display_set_name_str      (ScalarDisplay *self, const char* name);

gchar                *scalar_display_get_units_str    (ScalarDisplay *self);
void                 scalar_display_set_units_str     (ScalarDisplay *self, const char* units);

gdouble              scalar_display_get_value         (ScalarDisplay *self);
void                 scalar_display_set_value         (ScalarDisplay *self, gdouble new_value);

const char*          scalar_display_get_format_str    (ScalarDisplay *self);
void                 scalar_display_set_format_str    (ScalarDisplay *self, const char* disp_format);

gboolean             scalar_display_get_uncalibrated  (ScalarDisplay *self);
void                 scalar_display_set_uncalibrated  (ScalarDisplay *self, gboolean is_uncalibrated);

gboolean             scalar_display_get_has_error     (ScalarDisplay *self);
void                 scalar_display_set_has_error     (ScalarDisplay *self, gboolean errored);

gboolean             scalar_display_get_value_oor     (ScalarDisplay *self);
void                 scalar_display_set_value_oor     (ScalarDisplay *self, gboolean is_oor);

G_END_DECLS
#endif  // GTKMIN_SCALAR_DISPLAY_H_
