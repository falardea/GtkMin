/*
 * Copyright (c) 2024
 * Created by rfalardeau on 7/1/2024. 
*/
#ifndef GTKMIN_SCALAR_DISPLAY_H_
#define GTKMIN_SCALAR_DISPLAY_H_
#include <gtk/gtk.h>
#ifdef __cplusplus
extern "C" {
#endif

G_BEGIN_DECLS
/*
#define SCALAR_TYPE_DISPLAY              ( scalar_display_get_type( ) )
G_DECLARE_FINAL_TYPE(ScalarDisplay, scalar_diaplay, SCALAR, DISPLAY, GObject)
*/

#define SCALAR_DISPLAY(obj)            G_TYPE_CHECK_INSTANCE_CAST(obj, scalar_display_get_type(), ScalarDisplay)
#define SCALAR_DISPLAY_CLASS(klass)    G_TYPE_CHECK_CLASS_CAST(klass, scalar_display_get_type(), ScalarDisplayClass)
#define SCALAR_IS_DISPLAY(obj)         G_TYPE_CHECK_INSTANCE_TYPE(obj, scalar_display_get_type())
typedef struct _ScalarDisplay          ScalarDisplay;
typedef struct _ScalarDisplayClass     ScalarDisplayClass;

struct _ScalarDisplay
{
   GObject        parent_object;
   GtkBox         *gbox;
   GtkLabel       *name_label;
   GtkLabel       *value_label;
   GtkLabel       *units_label;

   gchar          *name;
   gchar          *units;
   gchar          *str_format;

   gdouble        value;
   gdouble        lo_limit;
   gdouble        hi_limit;

};

struct _ScalarDisplayClass
{
   GtkWidgetClass parent_class;

   void (*scalar_display_clicked)(ScalarDisplay *scalar);  // When the user clicks/touches
};

GtkWidget*           scalar_display_new            (const char* scalar_name,
                                                    const char* scalar_units,
                                                    const char* format_str,
                                                    gdouble lo_limit,
                                                    gdouble hi_limit);

gchar                *scalar_display_get_name_str   (ScalarDisplay *self);
void                 scalar_display_set_name_str   (ScalarDisplay *self, const char* name);

gchar                *scalar_display_get_units_str   (ScalarDisplay *self);
void                 scalar_display_set_units_str   (ScalarDisplay *self, const char* units);

gdouble              scalar_display_get_value      (ScalarDisplay *self);
void                 scalar_display_set_value      (ScalarDisplay *self, gdouble new_value);

gdouble              scalar_display_get_lo_limit   (ScalarDisplay *self);
void                 scalar_display_set_lo_limit   (ScalarDisplay *self, gdouble new_lo_limit);

gdouble              scalar_display_get_hi_limit   (ScalarDisplay *self);
void                 scalar_display_set_hi_limit   (ScalarDisplay *self, gdouble new_hi_limit);

const char*          scalar_display_get_str_format (ScalarDisplay *self);
void                 scalar_display_set_str_format (ScalarDisplay *self, const char* disp_format);

G_END_DECLS
#ifdef __cplusplus
}  /* closing brace for extern "C" */
#endif
#endif  // GTKMIN_SCALAR_DISPLAY_H_
