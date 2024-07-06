/*
 * Copyright (c) 2024
 * Created by rfalardeau on 7/1/2024.
*/
#include <glib-object.h>
#include <math.h>
#include "scalar_display.h"
#include "utils/logging.h"

enum {
   SCALAR_DISPLAY_CLICKED_SIGNAL,
   SCALAR_DISPLAY_N_SIGNALS
};

enum {
   PROP_0, // reserved for GObject
   SCALAR_DISPLAY_PROP_NAME_STR,
   SCALAR_DISPLAY_PROP_UNITS_STR,
   SCALAR_DISPLAY_PROP_VALUE,
   SCALAR_DISPLAY_PROP_LO_LIMIT,
   SCALAR_DISPLAY_PROP_HI_LIMIT,
   SCALAR_DISPLAY_PROP_STR_FORMAT,
   SCALAR_DISPLAY_N_PROPERTIES
};

//G_DEFINE_TYPE(ScalarDisplay, scalar_display, G_TYPE_OBJECT)
static void scalar_display_class_init(ScalarDisplayClass *klass, gpointer class_data);
static void scalar_display_init(ScalarDisplay *self, gpointer g_class);

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
      case SCALAR_DISPLAY_PROP_LO_LIMIT:
         scalar_display_set_lo_limit(scalar, g_value_get_double( value ) );
         break;
      case SCALAR_DISPLAY_PROP_HI_LIMIT:
         scalar_display_set_hi_limit(scalar, g_value_get_double( value ) );
         break;
      case SCALAR_DISPLAY_PROP_STR_FORMAT:
         scalar_display_set_format_str(scalar, g_value_get_string( value ));
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
      case SCALAR_DISPLAY_PROP_LO_LIMIT:
         g_value_set_double(value, scalar_display_get_lo_limit(scalar));
         break;
      case SCALAR_DISPLAY_PROP_HI_LIMIT:
         g_value_set_double(value, scalar_display_get_hi_limit(scalar));
         break;
      case SCALAR_DISPLAY_PROP_STR_FORMAT:
         g_value_set_string(value, scalar_display_get_format_str(scalar));
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static GParamSpec *scalar_display_properties[SCALAR_DISPLAY_N_PROPERTIES] = {NULL, };
static guint scalar_display_signals[SCALAR_DISPLAY_N_SIGNALS] = { 0 };

// static GtkWidgetClass *parent_class = NULL;
static void scalar_display_class_init(ScalarDisplayClass *klass,__attribute__((unused))  gpointer class_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   // parent_class = g_type_class_ref(gtk_widget_get_type());

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS (klass), "/mini_app/resources/scalar_display_template.glade");
   gtk_widget_class_bind_template_child(widget_class, ScalarDisplay, gbox);
   gtk_widget_class_bind_template_child(widget_class, ScalarDisplay, name_label);
   gtk_widget_class_bind_template_child(widget_class, ScalarDisplay, value_label);
   gtk_widget_class_bind_template_child(widget_class, ScalarDisplay, units_label);

   // gobject_class->dispose = scalar_display_dispose;
   gobject_class->finalize = scalar_display_finalize;

   gobject_class->get_property = scalar_display_get_property;
   gobject_class->set_property = scalar_display_set_property;

   scalar_display_properties[SCALAR_DISPLAY_PROP_VALUE] = g_param_spec_double("value", "scalar-display-value", "The scalar value",
                                                                              -INFINITY, INFINITY, 0, G_PARAM_READWRITE );
   scalar_display_properties[SCALAR_DISPLAY_PROP_LO_LIMIT] = g_param_spec_double("lo_limit", "scalar lo limit", "A lower threshold for flagging OOB values",
                                                                                 -INFINITY, INFINITY, 0, G_PARAM_READWRITE );
   scalar_display_properties[SCALAR_DISPLAY_PROP_HI_LIMIT] = g_param_spec_double("hi_limit", "scalar hi limit", "A higher threshold for flagging OOB values",
                                                                                 -INFINITY, INFINITY, 0, G_PARAM_READWRITE );
   scalar_display_properties[SCALAR_DISPLAY_PROP_STR_FORMAT] = g_param_spec_string("str_format", "formatting specification", "The format for displaying the scalar",
                                                                                   NULL, G_PARAM_READWRITE );
   scalar_display_properties[SCALAR_DISPLAY_PROP_NAME_STR] = g_param_spec_string("name_str", "scalar name", "The scalar name",
                                                                                 NULL, G_PARAM_READWRITE );
   scalar_display_properties[SCALAR_DISPLAY_PROP_UNITS_STR] = g_param_spec_string("units_str", "scalar units", "The scalar units",
                                                                                  NULL, G_PARAM_READWRITE );

   g_object_class_install_properties( gobject_class, SCALAR_DISPLAY_N_PROPERTIES, scalar_display_properties);
   scalar_display_signals[SCALAR_DISPLAY_CLICKED_SIGNAL] = g_signal_new("scalar-clicked",
                                                                        G_TYPE_FROM_CLASS(klass),
                                                                        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                                                                        G_STRUCT_OFFSET (ScalarDisplayClass , scalar_display_clicked),
                                                                        NULL,
                                                                        NULL,
                                                                        g_cclosure_marshal_VOID__VOID,
                                                                        G_TYPE_NONE, 0);
}

/////////////////// INSTANCE //////////////////////////////
static void scalar_display_init(ScalarDisplay *self,__attribute__((unused))  gpointer g_class)
{
   gtk_widget_init_template(GTK_WIDGET(self));
   scalar_display_set_name_str(self, "UNNAMED");
   scalar_display_set_units_str(self, "UOM");
   scalar_display_set_format_str(self, "%f");
   scalar_display_set_lo_limit(self, -INFINITY);
   scalar_display_set_hi_limit(self, INFINITY);
}

GtkWidget* scalar_display_new(const char* scalar_name,
                              const char* scalar_units,
                              const char* format_str,
                              gdouble lo_limit,
                              gdouble hi_limit)
{
   ScalarDisplay *scalar;
   scalar = g_object_new(scalar_display_get_type(), NULL);

   if(scalar_name && strcmp(scalar_name,"") != 0)
   {
      logging_llprintf(LOGLEVEL_DEBUG, "%s >>>>>>>>>>> setting name to %s", __func__, scalar_name);
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

   scalar_display_set_lo_limit(scalar, lo_limit);
   scalar_display_set_hi_limit(scalar, hi_limit);

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

/*
 *
 */
// static void scalar_display_dispose( GObject *self )
// {
//    g_return_if_fail(self != NULL);
//    g_return_if_fail(SCALAR_IS_DISPLAY(self));
//
//    ScalarDisplay *scalar = SCALAR_DISPLAY( self );
//
//    if (G_OBJECT_CLASS(SCALAR_DISPLAY_GET_CLASS(scalar))->dispose)
//    {
//       (* G_OBJECT_CLASS (SCALAR_DISPLAY_GET_CLASS(scalar))->dispose) (self);
//    }
// }

gchar *scalar_display_get_name_str (ScalarDisplay *self)
{
   g_return_val_if_fail(SCALAR_IS_DISPLAY(self), NULL);
   return g_strdup(self->name_str);
}
void scalar_display_set_name_str (ScalarDisplay *self, const char* name)
{
   g_return_if_fail(name);
   g_return_if_fail(SCALAR_IS_DISPLAY(self));
   if(self->name_str != NULL)
   {
      g_free(self->name_str);
   }
   self->name_str = g_strdup(name);
}

gchar *scalar_display_get_units_str (ScalarDisplay *self)
{
   g_return_val_if_fail(SCALAR_IS_DISPLAY(self), NULL);
   return g_strdup(self->units_str);
}
void  scalar_display_set_units_str (ScalarDisplay *self, const char* units)
{
   g_return_if_fail(units);
   g_return_if_fail(SCALAR_IS_DISPLAY(self));
   if(self->units_str != NULL)
      g_free(self->units_str);
   self->units_str = g_strdup(units);
}

const char* scalar_display_get_format_str(ScalarDisplay *self)
{
   g_return_val_if_fail(SCALAR_IS_DISPLAY(self), NULL);
   return g_strdup(self->format_str);
}
void scalar_display_set_format_str(ScalarDisplay *self, const char* disp_format)
{
   g_return_if_fail(disp_format);
   g_return_if_fail(SCALAR_IS_DISPLAY(self));
   if(self->format_str != NULL)
      g_free(self->format_str);
   self->format_str = g_strdup(disp_format);
}

gdouble scalar_display_get_value(ScalarDisplay *self)
{
   g_return_val_if_fail(SCALAR_IS_DISPLAY(self), NAN);
   return self->value;
}
void scalar_display_set_value(ScalarDisplay *self, gdouble new_value)
{
   g_return_if_fail(SCALAR_IS_DISPLAY(self));
   self->value = new_value;

   char value_str[28];
   snprintf(value_str, sizeof (value_str), self->format_str, self->value);
   gtk_label_set_label(GTK_LABEL(self->value_label), value_str);

   // In case there's something listening/bound?
   // g_object_notify_by_pspec(G_OBJECT(self), scalar_display_properties[SCALAR_DISPLAY_PROP_VALUE]);
}

gdouble scalar_display_get_lo_limit(ScalarDisplay *self)
{
   g_return_val_if_fail(SCALAR_IS_DISPLAY(self), NAN);
   return self->lo_limit;
}
void scalar_display_set_lo_limit(ScalarDisplay *self, gdouble new_lo_limit)
{
   g_return_if_fail(SCALAR_IS_DISPLAY(self));
   self->lo_limit = new_lo_limit;
   // In case there's something listening/bound?
   // g_object_notify_by_pspec(G_OBJECT(self), scalar_display_properties[SCALAR_DISPLAY_PROP_LO_LIMIT]);
}

gdouble scalar_display_get_hi_limit(ScalarDisplay *self)
{
   g_return_val_if_fail(SCALAR_IS_DISPLAY(self), NAN);
   return self->hi_limit;
}
void scalar_display_set_hi_limit(ScalarDisplay *self, gdouble new_hi_limit)
{
   g_return_if_fail(SCALAR_IS_DISPLAY(self));
   self->hi_limit = new_hi_limit;
   // In case there's something listening/bound?
   // g_object_notify_by_pspec(G_OBJECT(self), scalar_display_properties[SCALAR_DISPLAY_PROP_HI_LIMIT]);
}

