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
//   SCALAR_DISPLAY_PROP_NAME_STR,
//   SCALAR_DISPLAY_PROP_UNITS_STR,
   SCALAR_DISPLAY_PROP_VALUE,
//   SCALAR_DISPLAY_PROP_FORMAT_PRECISION,
//   SCALAR_DISPLAY_PROP_HAS_ERROR,
//   SCALAR_DISPLAY_PROP_VALUE_OOR,
//   SCALAR_DISPLAY_PROP_UNCALIBRATED,
   N_SCALAR_DISPLAY_PROPERTIES
};

struct _ScalarDisplay
{
   GtkBox         parent;
   GtkLabel       *name_label;
   GtkLabel       *value_label;
   GtkLabel       *units_label;

   gchar          *name_str;
   gchar          *units_str;
   guint          format_precision;

   gdouble        value;

   gboolean       has_error;
   gboolean       value_oor;
   gboolean       uncalibrated;
};

static guint scalar_display_signals[N_SCALAR_DISPLAY_SIGNALS] = { 0 };

static GParamSpec *scalar_display_properties[N_SCALAR_DISPLAY_PROPERTIES] = {NULL, };

G_DEFINE_TYPE(ScalarDisplay, scalar_display, GTK_TYPE_BOX)

// This is the function signature for callback listening for a SCALAR_DISPLAY_PRESSED_SIGNAL
void (* scalar_display_pressed) (ScalarDisplay *scalar, SCALAR_DISPLAY_MEASUREMENT_TYPES *src);

gboolean scalar_display_template_click_handler(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
   g_return_val_if_fail(widget != NULL, FALSE);

   if (event->type == GDK_BUTTON_PRESS)
   {
      ScalarDisplay *scalar = SCALAR_DISPLAY(user_data);
      g_signal_emit(G_OBJECT(scalar), scalar_display_signals[SCALAR_DISPLAY_PRESSED_SIGNAL], 0, SCALAR_DISPLAY_ARTERIAL_PRESSURE);
      return TRUE;
   }
   return FALSE;
}

static void scalar_display_update_label_text(ScalarDisplay *self);

static void scalar_display_finalize( GObject *self );

static void scalar_display_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
   ScalarDisplay *scalar = SCALAR_DISPLAY(object);

   g_assert_cmpint(prop_id, !=, 0);
   g_assert_true(prop_id < N_SCALAR_DISPLAY_PROPERTIES && pspec == scalar_display_properties[prop_id]);

   switch(prop_id){
//      case SCALAR_DISPLAY_PROP_NAME_STR:
//         scalar_display_set_name_str( scalar, g_value_get_string ( value ) );
//         break;
//      case SCALAR_DISPLAY_PROP_UNITS_STR:
//         scalar_display_set_units_str( scalar, g_value_get_string ( value ) );
//         break;
      case SCALAR_DISPLAY_PROP_VALUE:
         scalar_display_set_value( scalar, g_value_get_double ( value ) );
         break;
//      case SCALAR_DISPLAY_PROP_FORMAT_PRECISION:
//         scalar_display_set_precision(scalar, g_value_get_uint( value ));
//         break;
//      case SCALAR_DISPLAY_PROP_HAS_ERROR:
//         scalar_display_set_has_error(scalar, g_value_get_boolean( value ));
//         break;
//      case SCALAR_DISPLAY_PROP_VALUE_OOR:
//         scalar_display_set_value_oor(scalar, g_value_get_boolean( value ));
//         break;
//      case SCALAR_DISPLAY_PROP_UNCALIBRATED:
//         scalar_display_set_uncalibrated(scalar, g_value_get_boolean( value ));
//         break;
      default:
         // TODO: this should never happen, do we warn, assert, both, neither?
         // https://docs.gtk.org/gobject/func.OBJECT_WARN_INVALID_PROPERTY_ID.html
         // OR
         // https://docs.gtk.org/glib/func.assert_not_reached.html
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
         g_assert_not_reached ();
   }
}

static void scalar_display_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
   ScalarDisplay *scalar = SCALAR_DISPLAY(object);

   g_assert_cmpint(prop_id, !=, 0);
   g_assert_true(prop_id < N_SCALAR_DISPLAY_PROPERTIES && pspec == scalar_display_properties[prop_id]);

   switch (prop_id){
//      case SCALAR_DISPLAY_PROP_NAME_STR:
//         g_value_set_string(value, scalar_display_get_name_str(scalar));
//         break;
//      case SCALAR_DISPLAY_PROP_UNITS_STR:
//         g_value_set_string(value, scalar_display_get_units_str(scalar));
//         break;
      case SCALAR_DISPLAY_PROP_VALUE:
         g_value_set_double(value, scalar_display_get_value(scalar));
         break;
//      case SCALAR_DISPLAY_PROP_FORMAT_PRECISION:
//         g_value_set_uint(value, scalar_display_get_precision(scalar));
//         break;
//      case SCALAR_DISPLAY_PROP_HAS_ERROR:
//         g_value_set_boolean(value, scalar_display_get_has_error(scalar));
//         break;
//      case SCALAR_DISPLAY_PROP_VALUE_OOR:
//         g_value_set_boolean(value, scalar_display_get_value_oor(scalar));
//         break;
//      case SCALAR_DISPLAY_PROP_UNCALIBRATED:
//         g_value_set_boolean(value, scalar_display_get_uncalibrated(scalar));
//         break;
      default:
         // TODO: this should never happen, do we warn or assert and crash?
         // https://docs.gtk.org/gobject/func.OBJECT_WARN_INVALID_PROPERTY_ID.html
         // OR
         // https://docs.gtk.org/glib/func.assert_not_reached.html
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
         g_assert_not_reached ();
   }
}

static void scalar_display_class_init(ScalarDisplayClass *klass)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS (klass), "/mini_app/resources/scalar_display_template.glade");
   gtk_widget_class_bind_template_child(widget_class, ScalarDisplay, name_label);
   gtk_widget_class_bind_template_child(widget_class, ScalarDisplay, value_label);
   gtk_widget_class_bind_template_child(widget_class, ScalarDisplay, units_label);
   gtk_widget_class_bind_template_callback_full(widget_class, "btn_press_event", (GCallback)scalar_display_template_click_handler);

    scalar_display_signals[SCALAR_DISPLAY_PRESSED_SIGNAL] = g_signal_new_class_handler("scalar-display-pressed",
                                                                                       G_TYPE_FROM_CLASS(klass),
                                                                                       G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                                                                                       (GCallback)scalar_display_pressed,
                                                                                       NULL,
                                                                                       NULL,
                                                                                       g_cclosure_marshal_VOID__INT,
                                                                                       G_TYPE_NONE, 1, G_TYPE_INT);

   gobject_class->finalize = scalar_display_finalize;
   gobject_class->get_property = scalar_display_get_property;
   gobject_class->set_property = scalar_display_set_property;

   scalar_display_properties[SCALAR_DISPLAY_PROP_VALUE] = g_param_spec_double("value", NULL, NULL, -INFINITY, INFINITY, 0, G_PARAM_READWRITE );
//   scalar_display_properties[SCALAR_DISPLAY_PROP_FORMAT_PRECISION] = g_param_spec_uint("format_precision", NULL, NULL, 0, G_MAXINT, 0, G_PARAM_READWRITE );
//   scalar_display_properties[SCALAR_DISPLAY_PROP_NAME_STR] = g_param_spec_string("name_str", NULL, NULL, NULL, G_PARAM_READWRITE );
//   scalar_display_properties[SCALAR_DISPLAY_PROP_UNITS_STR] = g_param_spec_string("units_str", NULL, NULL, NULL, G_PARAM_READWRITE );
//
//   scalar_display_properties[SCALAR_DISPLAY_PROP_HAS_ERROR] = g_param_spec_boolean("has_error", NULL, NULL, FALSE, G_PARAM_READWRITE );
//   scalar_display_properties[SCALAR_DISPLAY_PROP_VALUE_OOR] = g_param_spec_boolean("value_oor", NULL, NULL, FALSE, G_PARAM_READWRITE );
//   scalar_display_properties[SCALAR_DISPLAY_PROP_UNCALIBRATED] = g_param_spec_boolean("uncalibrated", NULL, NULL, TRUE, G_PARAM_READWRITE );

   g_object_class_install_properties( gobject_class, N_SCALAR_DISPLAY_PROPERTIES, scalar_display_properties);
}

/////////////////// INSTANCE //////////////////////////////
static void scalar_display_init(ScalarDisplay *self)
{
//   gtk_widget_add_events(GTK_WIDGET(self), GDK_BUTTON_PRESS_MASK);

   gtk_widget_init_template(GTK_WIDGET(self));

   scalar_display_set_has_error(self, FALSE);
   scalar_display_set_value_oor(self, FALSE); // True on startup??
   scalar_display_set_uncalibrated(self, TRUE);

   scalar_display_set_value(self, NAN);
}

GtkWidget* scalar_display_new(const char* scalar_name, const char* scalar_units, guint scalar_precision)
{
   ScalarDisplay *scalar;
   scalar = g_object_new(scalar_display_get_type(), NULL);

   scalar_display_set_name_str(scalar, scalar_name);

   scalar_display_set_units_str(scalar, scalar_units);

   scalar_display_set_precision(scalar, scalar_precision);

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

   G_OBJECT_CLASS(scalar_display_parent_class)->finalize (self);
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
   gtk_label_set_label(GTK_LABEL(self->name_label), self->name_str);
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
//   g_return_if_fail(units != NULL);

   if(self->units_str != NULL)
      g_free(self->units_str);
   self->units_str = g_strdup(units);
   gtk_label_set_label(GTK_LABEL(self->units_label), self->units_str);
}

guint scalar_display_get_precision(ScalarDisplay *self)
{
   g_return_val_if_fail(self != NULL, 0);
   g_return_val_if_fail(SCALAR_IS_DISPLAY(self), 0);

   return self->format_precision;
}
void scalar_display_set_precision(ScalarDisplay *self, guint scalar_precision)
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(SCALAR_IS_DISPLAY(self));

   self->format_precision = scalar_precision;
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
      snprintf(value_str, sizeof (value_str), "%.*f", self->format_precision, self->value);
   }
   else
   {
      snprintf(value_str, sizeof (value_str), "%s", "NAN");
   }

   gchar label_str[100];
   if (scalar_display_get_has_error(self))
   {
      snprintf(label_str, sizeof(label_str), "<span foreground='red'>ERROR</span>");
   }
   else if (scalar_display_get_value_oor(self))
   {
      snprintf(label_str, sizeof(label_str), "<span foreground='red'>%s</span>", value_str);
   }
   else if (scalar_display_get_uncalibrated(self))
   {
      snprintf(label_str, sizeof(label_str), "<span foreground='orange'>%s</span>", value_str);
   }
   else
   {
      snprintf(label_str, sizeof(label_str), "%s", value_str);
   }
   gtk_label_set_markup(GTK_LABEL(self->value_label), label_str);
}


