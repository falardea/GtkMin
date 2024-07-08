/*
 * Created by french on 7/5/24.
*/
#include <math.h>
#include "numeric_label.h"
#include "../utils/logging.h"

struct _NumericLabel
{
   GtkBin      parent;
   GtkLabel    *label_ref;

   gdouble     value;
   gchar       *format_str;
};

// struct _NumericLabelClass
// {
//    GtkBinClass parent_class;
// };

enum {
   PROP_O,
   NUMERIC_LABEL_VALUE_PROP,
   NUMERIC_LABEL_FORMAT_STR_PROP,
   N_NUMERIC_LABEL_PROPERTIES
};

G_DEFINE_TYPE(NumericLabel, numeric_label, GTK_TYPE_BIN )

static void numeric_label_finalize( GObject *object );

static void numeric_label_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
   NumericLabel *self = NUMERIC_LABEL(object);
   switch(prop_id){
      case NUMERIC_LABEL_VALUE_PROP:
         numeric_label_set_value(self, g_value_get_double( value ) );
         break;
      case NUMERIC_LABEL_FORMAT_STR_PROP:
         numeric_label_set_format_str(self, g_value_get_string( value ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void numeric_label_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
   NumericLabel *self = NUMERIC_LABEL(object);
   switch(prop_id)
   {
      case NUMERIC_LABEL_VALUE_PROP:
         g_value_set_double( value, numeric_label_get_value(self));
         break;
      case NUMERIC_LABEL_FORMAT_STR_PROP:
         g_value_set_string( value, numeric_label_get_format_str(self));
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static GParamSpec *numeric_properties[N_NUMERIC_LABEL_PROPERTIES] = {NULL, };

static void numeric_label_class_init(NumericLabelClass *klass)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS (klass), "/mini_app/resources/numeric_label.glade");
   gtk_widget_class_bind_template_child(widget_class, NumericLabel , label_ref);

   gobject_class->finalize = numeric_label_finalize;
   gobject_class->get_property = numeric_label_get_property;
   gobject_class->set_property = numeric_label_set_property;
   numeric_properties[NUMERIC_LABEL_VALUE_PROP] = g_param_spec_double("value", "value", "value",
                                                                      -G_MINDOUBLE,
                                                                      G_MAXDOUBLE,
                                                                      0.0,
                                                                      G_PARAM_READWRITE );
   numeric_properties[NUMERIC_LABEL_FORMAT_STR_PROP] = g_param_spec_string("format_str", "format_str", "format_str",
                                                                           NULL,
                                                                           G_PARAM_READWRITE);
   g_object_class_install_properties(gobject_class, N_NUMERIC_LABEL_PROPERTIES, numeric_properties);
}

//// Instance ////////////
static void numeric_label_init(NumericLabel *self)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   gtk_widget_init_template(GTK_WIDGET(self));
   numeric_label_set_value(self, 0.0);
   numeric_label_set_format_str(self, "%f");
}

GtkWidget *numeric_label_new(gdouble init_value, gchar *init_format_str)
{
   NumericLabel *label;
   label = g_object_new(numeric_label_get_type(), NULL);

   if (init_format_str != NULL && g_strcmp0(init_format_str, ""))
   {
      numeric_label_set_format_str(label, init_format_str);
   }

   // Always set the initial value AFTER setting the format
   if (!isnan(init_value))
   {
      numeric_label_set_value(label, init_value);
   }

   return GTK_WIDGET(label);
}

static void numeric_label_finalize( GObject *object )
{
   g_return_if_fail(object != NULL);
   g_return_if_fail(NUMERIC_IS_LABEL(object));

   NumericLabel *label = NUMERIC_LABEL(object);
   if(label->format_str != NULL) g_free(label->format_str);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

gdouble numeric_label_get_value(NumericLabel *self)
{
   g_return_val_if_fail(NUMERIC_IS_LABEL(self), NAN);
   return self->value;
}
void numeric_label_set_value(NumericLabel *self, gdouble new_value)
{
   g_return_if_fail(NUMERIC_IS_LABEL(self));
   self->value = new_value;

   char value_str[28];
   snprintf(value_str, sizeof (value_str), self->format_str, self->value);
   gtk_label_set_label(GTK_LABEL(self->label_ref), value_str);

   // In case there's something listening/bound?
   g_object_notify_by_pspec(G_OBJECT(self), numeric_properties[NUMERIC_LABEL_VALUE_PROP]);
}

gchar *numeric_label_get_format_str(NumericLabel *self)
{
   g_return_val_if_fail(NUMERIC_IS_LABEL(self), NULL);
   return g_strdup(self->format_str);
}
void numeric_label_set_format_str(NumericLabel *self, const gchar *new_format_st)
{
   g_return_if_fail(new_format_st);
   g_return_if_fail(NUMERIC_IS_LABEL(self));
   if(self->format_str != NULL)
      g_free(self->format_str);
   self->format_str = g_strdup(new_format_st);
}
