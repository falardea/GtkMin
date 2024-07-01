/*
 * Copyright (c) 2024
 * Created by rfalardeau on 7/1/2024. 
*/
#include <glib-object.h>
#include <math.h>
#include "scalar_display.h"

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

struct _ScalarDisplay
{
   gdouble        value;
   gdouble        lo_limit;
   gdouble        hi_limit;

   GtkBin         *root_container;
   GtkLabel       *name_label;
   GtkLabel       *value_label;
   GtkLabel       *units_label;

   gchar          *name;
   gchar          *units;
   gchar          *str_format;
};

struct _ScalarDisplayClass
{
   GtkWidgetClass parent_class;

   void (*scalar_display_clicked)(ScalarDisplay *scalar);  // When the user clicks/touches
};

G_DEFINE_TYPE(ScalarDisplay, scalar_display, G_TYPE_OBJECT)

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
         scalar_display_set_str_format(scalar, g_value_get_string( value ));
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
         g_value_set_double(value, scalar_display_get_name_str(scalar));
         break;
      case SCALAR_DISPLAY_PROP_UNITS_STR:
         g_value_set_double(value, scalar_display_get_units_str(scalar));
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
         g_value_set_string(value, scalar_display_get_str_format(scalar));
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static GParamSpec *scalar_display_properties[SCALAR_DISPLAY_N_PROPERTIES] = {NULL, };
static guint scalar_display_signals[SCALAR_DISPLAY_N_SIGNALS] = { 0 };

static void scalar_display_class_init(ScalarDisplayClass *klass)
{
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   gobject_class->finalize = scalar_display_finalize;

   gobject_class->get_property = scalar_display_get_property;
   gobject_class->set_property = scalar_display_set_property;

   scalar_display_properties[SCALAR_DISPLAY_PROP_VALUE] = g_param_spec_double("value", "scalar-display-value", "The scalar value",
                                                          -INFINITY, INFINITY, NAN, G_PARAM_READWRITE );
   scalar_display_properties[SCALAR_DISPLAY_PROP_LO_LIMIT] = g_param_spec_double("lo_limit", "scalar lo limit", "A lower threshold for flagging OOB values",
                                                                                 -INFINITY, INFINITY, NAN, G_PARAM_READWRITE );
   scalar_display_properties[SCALAR_DISPLAY_PROP_HI_LIMIT] = g_param_spec_double("hi_limit", "scalar hi limit", "A higher threshold for flagging OOB values",
                                                                                 -INFINITY, INFINITY, NAN, G_PARAM_READWRITE );
   scalar_display_properties[SCALAR_DISPLAY_PROP_STR_FORMAT] = g_param_spec_string("str_format", "formatting specification", "The format for displaying the scalar",
                                                                                   NULL, G_PARAM_READWRITE );
   scalar_display_properties[SCALAR_DISPLAY_PROP_NAME_STR] = g_param_spec_string("name_str", "scalar name", "The scalar name",
                                                                                 NULL, G_PARAM_READWRITE );
   scalar_display_properties[SCALAR_DISPLAY_PROP_UNITS_STR] = g_param_spec_string("units_str", "scalar units", "The scalar units",
                                                                                  NULL, G_PARAM_READWRITE );

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
static void scalar_display_init(ScalarDisplay *self)
{

}

GtkWidget* scalar_display_new(const char* scalar_name,
                              const char* scalar_units,
                              const char* format_str,
                              gdouble lo_limit,
                              gdouble hi_limit)
{
   ScalarDisplay *scalar;
   scalar = g_object_new(scalar_display_get_type(), NULL);

   if(scalar && strcmp(scalar_name,"") == 0)
   {

   }
}

static void scalar_display_finalize( GObject *self ){}

gchar scalar_display_get_name_str (ScalarDisplay *self){}
void scalar_display_set_name_str (ScalarDisplay *self, const char* name){}

gchar scalar_display_get_units_str (ScalarDisplay *self){}
void  scalar_display_set_units_str (ScalarDisplay *self, const char* units){}

gdouble scalar_display_get_value(ScalarDisplay *self){}
void scalar_display_set_value(ScalarDisplay *self, gdouble new_value){}

gdouble scalar_display_get_lo_limit(ScalarDisplay *self){}
void scalar_display_set_lo_limit(ScalarDisplay *self, gdouble new_lo_limit){}

gdouble scalar_display_get_hi_limit(ScalarDisplay *self){}
void scalar_display_set_hi_limit(ScalarDisplay *self, gdouble new_hi_limit){}

const char* scalar_display_get_str_format(ScalarDisplay *self){}
void scalar_display_set_str_format(ScalarDisplay *self, const char* disp_format){}
