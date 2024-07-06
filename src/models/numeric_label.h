/*
 * Created by french on 7/5/24.
*/
#ifndef GTKMIN_NUMERIC_LABEL_H_
#define GTKMIN_NUMERIC_LABEL_H_
#include <gtk/gtk.h>
#include <glib-object.h>

G_BEGIN_DECLS // C++ guard, from gmacros.h

#define NUMERIC_TYPE_LABEL ( numeric_label_get_type () )

G_DECLARE_FINAL_TYPE(NumericLabel, numeric_label, NUMERIC, LABEL, GtkBin)

GtkWidget *numeric_label_new(gdouble init_value, gchar *init_format_str);

gdouble numeric_label_get_value(NumericLabel *self);
void numeric_label_set_value(NumericLabel *self, gdouble new_value);

gchar *numeric_label_get_format_str(NumericLabel *self);
void numeric_label_set_format_str(NumericLabel *self, const gchar *new_format_st);

G_END_DECLS
#endif  // GTKMIN_NUMERIC_LABEL_H_