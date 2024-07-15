/*
 * Copyright (c) 2024
 * Created by rfalardeau on 7/1/2024.
*/
#include <glib-object.h>
#include <math.h>
#include "scalar_display.h"
#include "utils/logging.h"

enum {
   SCALAR_DISPLAY_PRESSED_SIGNAL,
   N_SCALAR_DISPLAY_SIGNALS
};

enum {
   PROP_0, // reserved for GObject
   SCALAR_DISPLAY_PROP_NAME_STR,
   SCALAR_DISPLAY_PROP_UNITS_STR,
   SCALAR_DISPLAY_PROP_VALUE,
   SCALAR_DISPLAY_PROP_STR_FORMAT,
   SCALAR_DISPLAY_PROP_HAS_ERROR,
   SCALAR_DISPLAY_PROP_VALUE_OOR,
   SCALAR_DISPLAY_PROP_UNCALIBRATED,
   N_SCALAR_DISPLAY_PROPERTIES
};

//G_DEFINE_TYPE(ScalarDisplay, scalar_display, GTK_TYPE_BOX)
static void scalar_display_class_init(ScalarDisplayClass *klass, gpointer class_data);
static void scalar_display_init(ScalarDisplay *self, gpointer g_class);

//gboolean scalar_display_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
static void scalar_display_update_label_text(ScalarDisplay *self);

GType scalar_display_get_type()
{
   static GType scalar_type = 0;
   if (!scalar_type)
   {
      static const GTypeInfo scalar_info =
            {
                  sizeof (ScalarDisplayClass),
                  NULL,
                  NULL,
                  (GClassInitFunc) scalar_display_class_init,
                  NULL,
                  NULL,
                  sizeof(ScalarDisplay),
                  0,
                  (GInstanceInitFunc) scalar_display_init,
                  NULL
            };
      // Note the parent type here and "parent/root" define in the instance and class structs
      scalar_type = g_type_register_static(GTK_TYPE_BOX, "ScalarDisplay", &scalar_info, 0);
   }
   return scalar_type;
}

// static void scalar_display_dispose( GObject *self );
static void scalar_display_finalize( GObject *self );

static void scalar_display_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
   ScalarDisplay *scalar = SCALAR_DISPLAY(object);

   switch(prop_id){
      case SCALAR_DISPLAY_PROP_NAME_STR:
         scalar_display_set_name_str( scalar, g_value_get_string ( value ) );
         break;
      case SCALAR_DISPLAY_PROP_UNITS_STR:
         scalar_display_set_units_str( scalar, g_value_get_string ( value ) );
         break;
      case SCALAR_DISPLAY_PROP_VALUE:
         scalar_display_set_value( scalar, g_value_get_double ( value ) );
         break;
      case SCALAR_DISPLAY_PROP_STR_FORMAT:
         scalar_display_set_format_str(scalar, g_value_get_string( value ));
         break;
      case SCALAR_DISPLAY_PROP_HAS_ERROR:
         scalar_display_set_has_error(scalar, g_value_get_boolean( value ));
         break;
      case SCALAR_DISPLAY_PROP_VALUE_OOR:
         scalar_display_set_value_oor(scalar, g_value_get_boolean( value ));
         break;
      case SCALAR_DISPLAY_PROP_UNCALIBRATED:
         scalar_display_set_uncalibrated(scalar, g_value_get_boolean( value ));
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }

}

static void scalar_display_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
   ScalarDisplay *scalar = SCALAR_DISPLAY(object);

   switch (prop_id){
      case SCALAR_DISPLAY_PROP_NAME_STR:
         g_value_set_string(value, scalar_display_get_name_str(scalar));
         break;
      case SCALAR_DISPLAY_PROP_UNITS_STR:
         g_value_set_string(value, scalar_display_get_units_str(scalar));
         break;
      case SCALAR_DISPLAY_PROP_VALUE:
         g_value_set_double(value, scalar_display_get_value(scalar));
         break;
      case SCALAR_DISPLAY_PROP_STR_FORMAT:
         g_value_set_string(value, scalar_display_get_format_str(scalar));
         break;
      case SCALAR_DISPLAY_PROP_HAS_ERROR:
         g_value_set_boolean(value, scalar_display_get_has_error(scalar));
         break;
      case SCALAR_DISPLAY_PROP_VALUE_OOR:
         g_value_set_boolean(value, scalar_display_get_value_oor(scalar));
         break;
      case SCALAR_DISPLAY_PROP_UNCALIBRATED:
         g_value_set_boolean(value, scalar_display_get_uncalibrated(scalar));
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

__attribute__((unused))
static guint gtk_dial_signals[N_SCALAR_DISPLAY_SIGNALS] = { 0 };
static GParamSpec *scalar_display_properties[N_SCALAR_DISPLAY_PROPERTIES] = {NULL, };

// static GtkWidgetClass *parent_class = NULL;

gboolean scalar_press_event(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   return TRUE;
}
static void scalar_display_class_init(ScalarDisplayClass *klass,__attribute__((unused)) gpointer class_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   // parent_class = g_type_class_ref(gtk_widget_get_type());

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS (klass), "/mini_app/resources/scalar_display_template.glade");
   // gtk_widget_class_bind_template_child(widget_class, ScalarDisplay, gbox);
   gtk_widget_class_bind_template_child(widget_class, ScalarDisplay, name_label);
   gtk_widget_class_bind_template_child(widget_class, ScalarDisplay, value_label);
   gtk_widget_class_bind_template_child(widget_class, ScalarDisplay, units_label);
//   gtk_widget_class_bind_template_callback(widget_class, scalar_display_button_press);

//   widget_class->button_press_event = scalar_display_button_press;
   gtk_widget_class_bind_template_callback_full(widget_class, "btn_press_event", (GCallback)scalar_press_event);

   // gtk_widget_class_bind_template_callback_full(widget_class, "scalar_display_button_press", (GCallback)scalar_display_button_press);

   //

   // gtk_dial_signals[SCALAR_DISPLAY_PRESSED_SIGNAL] = g_signal_new("button-press-event",
   //                                                      G_TYPE_FROM_CLASS(klass),
   //                                                      G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
   //                                                      G_STRUCT_OFFSET (GtkWidgetClass , button_press_event),
   //                                                      NULL,
   //                                                      NULL,
   //                                                      g_cclosure_marshal_VOID__DOUBLE,
   //                                                      G_TYPE_NONE, 1, G_TYPE_DOUBLE);

   gobject_class->finalize = scalar_display_finalize;

   gobject_class->get_property = scalar_display_get_property;
   gobject_class->set_property = scalar_display_set_property;

   scalar_display_properties[SCALAR_DISPLAY_PROP_VALUE] = g_param_spec_double("value", NULL, NULL, -INFINITY, INFINITY, 0, G_PARAM_READWRITE );
   scalar_display_properties[SCALAR_DISPLAY_PROP_STR_FORMAT] = g_param_spec_string("str_format", NULL, NULL, NULL, G_PARAM_READWRITE );
   scalar_display_properties[SCALAR_DISPLAY_PROP_NAME_STR] = g_param_spec_string("name_str", NULL, NULL, NULL, G_PARAM_READWRITE );
   scalar_display_properties[SCALAR_DISPLAY_PROP_UNITS_STR] = g_param_spec_string("units_str", NULL, NULL, NULL, G_PARAM_READWRITE );

   scalar_display_properties[SCALAR_DISPLAY_PROP_HAS_ERROR] = g_param_spec_boolean("has_error", NULL, NULL, FALSE, G_PARAM_READWRITE );
   scalar_display_properties[SCALAR_DISPLAY_PROP_VALUE_OOR] = g_param_spec_boolean("value_oor", NULL, NULL, FALSE, G_PARAM_READWRITE );
   scalar_display_properties[SCALAR_DISPLAY_PROP_UNCALIBRATED] = g_param_spec_boolean("uncalibrated", NULL, NULL, TRUE, G_PARAM_READWRITE );

   g_object_class_install_properties( gobject_class, N_SCALAR_DISPLAY_PROPERTIES, scalar_display_properties);
}

/////////////////// INSTANCE //////////////////////////////
static void scalar_display_init(ScalarDisplay *self,__attribute__((unused)) gpointer g_class)
{
   gtk_widget_add_events(GTK_WIDGET(self), GDK_BUTTON_PRESS_MASK);

   gtk_widget_init_template(GTK_WIDGET(self));

   scalar_display_set_name_str(self, "UNNAMED");
   scalar_display_set_units_str(self, "UOM");
   scalar_display_set_format_str(self, "%f");
   scalar_display_set_has_error(self, FALSE);
   scalar_display_set_value_oor(self, FALSE); // True on startup??
   scalar_display_set_uncalibrated(self, TRUE);

   scalar_display_set_value(self, NAN);
}

GtkWidget* scalar_display_new(const char* scalar_name, const char* scalar_units, const char* format_str)
{
   ScalarDisplay *scalar;
   scalar = g_object_new(scalar_display_get_type(), NULL);

   if(scalar_name && strcmp(scalar_name,"") != 0)
   {
      scalar_display_set_name_str(scalar, scalar_name);
   }
   gtk_label_set_label(GTK_LABEL(scalar->name_label), scalar->name_str);

   if(scalar_units && strcmp(scalar_units,"") != 0)
   {
      scalar_display_set_units_str(scalar, scalar_units);
   }
   gtk_label_set_label(GTK_LABEL(scalar->units_label), scalar->units_str);

   if(format_str && strcmp(format_str, "") != 0)
   {
      scalar_display_set_format_str(scalar, format_str);
   }

//   g_signal_connect(scalar, "button-press-event", G_CALLBACK(scalar_display_button_press), g_app_widget_refs);
   gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(scalar)), "scalar_display");

   return GTK_WIDGET(scalar);
}

static void scalar_display_finalize( GObject *self )
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(SCALAR_IS_DISPLAY(self));
   ScalarDisplay *scalar = SCALAR_DISPLAY(self );

   if (scalar->name_str != NULL) g_free(scalar->name_str);
   if (scalar->units_str != NULL) g_free(scalar->units_str);
   if (scalar->format_str != NULL) g_free(scalar->format_str);

   // GObjectClass *parent_class = G_OBJECT_CLASS( SCALAR_DISPLAY_GET_CLASS(scalar) );
   // ( *parent_class->finalize )( self );
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

gchar *scalar_display_get_name_str (ScalarDisplay *self)
{
   g_return_val_if_fail(self != NULL, NULL);
   g_return_val_if_fail(SCALAR_IS_DISPLAY(self), NULL);

   return g_strdup(self->name_str);
}
void scalar_display_set_name_str (ScalarDisplay *self, const char* name)
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(SCALAR_IS_DISPLAY(self));
   g_return_if_fail(name != NULL);

   if(self->name_str != NULL)
   {
      g_free(self->name_str);
   }
   self->name_str = g_strdup(name);
}

gchar *scalar_display_get_units_str (ScalarDisplay *self)
{
   g_return_val_if_fail(self != NULL, NULL);
   g_return_val_if_fail(SCALAR_IS_DISPLAY(self), NULL);

   return g_strdup(self->units_str);
}
void  scalar_display_set_units_str (ScalarDisplay *self, const char* units)
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(SCALAR_IS_DISPLAY(self));
   g_return_if_fail(units != NULL);

   if(self->units_str != NULL)
      g_free(self->units_str);
   self->units_str = g_strdup(units);
}

const char* scalar_display_get_format_str(ScalarDisplay *self)
{
   g_return_val_if_fail(self != NULL, NULL);
   g_return_val_if_fail(SCALAR_IS_DISPLAY(self), NULL);

   return g_strdup(self->format_str);
}
void scalar_display_set_format_str(ScalarDisplay *self, const char* disp_format)
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(SCALAR_IS_DISPLAY(self));
   g_return_if_fail(disp_format != NULL);

   if(self->format_str != NULL)
      g_free(self->format_str);
   self->format_str = g_strdup(disp_format);
}

gdouble scalar_display_get_value(ScalarDisplay *self)
{
   g_return_val_if_fail(self != NULL, NAN);
   g_return_val_if_fail(SCALAR_IS_DISPLAY(self), NAN);

   return self->value;
}
void scalar_display_set_value(ScalarDisplay *self, gdouble new_value)
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(SCALAR_IS_DISPLAY(self));
   self->value = new_value;
   scalar_display_update_label_text(self);
}

gboolean scalar_display_get_uncalibrated(ScalarDisplay *self)
{
   g_return_val_if_fail(self != NULL, FALSE);
   g_return_val_if_fail(SCALAR_IS_DISPLAY(self), FALSE);
   return self->uncalibrated;
}
void scalar_display_set_uncalibrated(ScalarDisplay *self, gboolean is_uncalibrated)
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(SCALAR_IS_DISPLAY(self));

   self->uncalibrated = is_uncalibrated;
   scalar_display_update_label_text(self);
}

gboolean scalar_display_get_has_error(ScalarDisplay *self)
{
   g_return_val_if_fail(self != NULL, FALSE);
   g_return_val_if_fail(SCALAR_IS_DISPLAY(self), FALSE);

   return self->has_error;
}
void scalar_display_set_has_error(ScalarDisplay *self, gboolean errored)
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(SCALAR_IS_DISPLAY(self));

   self->has_error = errored;
   scalar_display_update_label_text(self);
}

gboolean scalar_display_get_value_oor(ScalarDisplay *self)
{
   g_return_val_if_fail(self != NULL, FALSE);
   g_return_val_if_fail(SCALAR_IS_DISPLAY(self), FALSE);

   return self->value_oor;
}
void scalar_display_set_value_oor(ScalarDisplay *self, gboolean is_oor)
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(SCALAR_IS_DISPLAY(self));

   self->value_oor = is_oor;
   scalar_display_update_label_text(self);
}

static void scalar_display_update_label_text(ScalarDisplay *self)
{
   char value_str[28];

   if (!isnan(self->value))
   {
      snprintf(value_str, sizeof (value_str), self->format_str, self->value);
   }
   else
   {
      snprintf(value_str, sizeof (value_str), "%s", "NAN");
   }

   gchar label_str[100];
   if (scalar_display_get_has_error(self))
   {
      snprintf(label_str, sizeof(label_str), "<span foreground='red'>ERROR</span>");
   } else if (scalar_display_get_value_oor(self))
   {
      snprintf(label_str, sizeof(label_str), "<span foreground='red'>%s</span>", value_str);
   } else if (scalar_display_get_uncalibrated(self))
   {
      snprintf(label_str, sizeof(label_str), "<span foreground='orange'>%s</span>", value_str);
   }
   else
   {
      snprintf(label_str, sizeof(label_str), "%s", value_str);
   }
   gtk_label_set_markup(GTK_LABEL(self->value_label), label_str);
}


