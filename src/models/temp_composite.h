/*
 * Copyright (c) 2024
 * Created by rfalardeau on 7/15/2024. 
*/

#ifndef TEMP_COMPOSITE_H___
#define TEMP_COMPOSITE_H___
#include "comm.h"
G_BEGIN_DECLS

#define TEMP_TYPE_COMPOSITE      ( temp_type_composite_get_type () )

G_DECLARE_FINAL_TYPE(TempComposite, temp_composite, TEMP, COMPOSITE, GtkBox)

GtkWidget *temp_composite_new(const gchar *new_value);

gchar *temp_composite_get_name(TempComposite *self);
void temp_composite_set_name(TempComposite *self, const gchar *new_value);

G_END_DECLS
#endif  // TEMP_COMPOSITE_H___
