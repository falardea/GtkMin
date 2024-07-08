/*
 * Created by french on 7/6/24.
*/
#include <math.h>
#include <cairo/cairo.h>
#include <glib-object.h>
#include "battery_indicator.h"

#include "utils/logging.h"

struct _BatteryIndicator
{
   GtkWidget            parent;
   // GtkWidget         *child_widget;
   gdouble              value;         // the percent charge
   gdouble              old_value;     // to determine change?
   guint                event_button;  // the gdk event->button, not a widget
};

/*struct _BatteryIndicatorClass
{
   GtkWidgetClass    parent_class;

   void  (*battery_indicator_pressed) (BatteryIndicator *batt, gdouble value);
};*/

// It appears that anything that isn't defined as a property is effectively private instance data?
enum {
   PROP_0,
   BATTERY_INDICATOR_VALUE_PROP,
   N_BATTERY_INDICATOR_PROPERTIES
};

/*enum {
   BATTERY_INDICATOR_PRESSED_SIGNAL,
   N_BATTERY_INDICATOR_SIGNALS
};*/

G_DEFINE_TYPE(BatteryIndicator, battery_indicator, GTK_TYPE_WIDGET)

static void       battery_indicator_realize                 (GtkWidget *widget);
static void       battery_indicator_size_allocate           (GtkWidget *widget,
                                                             GtkAllocation *allocation);
static gboolean   battery_indicator_draw                    (GtkWidget *widget,
                                                             cairo_t *cr);

static void       battery_indicator_finalize       ( GObject *object );
static gboolean   battery_indicator_button_press   (GtkWidget *widget,
                                                    GdkEventButton *event);

static void battery_indicator_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
   BatteryIndicator *self = (BatteryIndicator *) object;
   switch (prop_id)
   {
      case BATTERY_INDICATOR_VALUE_PROP:
         battery_indicator_set_value(object, g_value_get_double( value ));
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
   }
}

static void battery_indicator_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
   BatteryIndicator *self = (BatteryIndicator *) object;

   switch (prop_id)
   {
      case BATTERY_INDICATOR_VALUE_PROP:
         g_value_set_double(value, battery_indicator_get_value(self));
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
   }
}

static GParamSpec *battery_indicator_properties[N_BATTERY_INDICATOR_PROPERTIES] = {NULL, };
// static guint battery_indicator_signals[N_BATTERY_INDICATOR_SIGNALS] = { 0 };

static void battery_indicator_class_init(BatteryIndicatorClass *klass)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   GObjectClass *object_class = G_OBJECT_CLASS(klass);

   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;
   // gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/mini_app/resources/battery_indicator.glade");
   // gtk_widget_class_bind_template_child(widget_class, BatteryIndicator, child_widget);

   widget_class->button_press_event = battery_indicator_button_press;
   widget_class->realize = battery_indicator_realize;
   widget_class->draw = battery_indicator_draw;
   widget_class->size_allocate = battery_indicator_size_allocate;

   object_class->finalize = battery_indicator_finalize;
   object_class->set_property = battery_indicator_set_property;
   object_class->get_property = battery_indicator_get_property;
   battery_indicator_properties[BATTERY_INDICATOR_VALUE_PROP] = g_param_spec_double("value", NULL, NULL,
                                                                                    -G_MINDOUBLE, G_MAXDOUBLE,
                                                                                    0, G_PARAM_READWRITE);
   g_object_class_install_properties(object_class, N_BATTERY_INDICATOR_PROPERTIES, battery_indicator_properties);
}

//// Instance ////////////
static void battery_indicator_init(BatteryIndicator *batt)
{
   batt->value = 0.0;
   batt->old_value = 0.0;
   batt->event_button = 0;
}

GtkWidget *battery_indicator_new(void)
{
   return g_object_new(battery_indicator_get_type(), NULL);
}

static void battery_indicator_finalize(GObject *object)
{
   g_return_if_fail(object != NULL);
   g_return_if_fail(BATTERY_IS_INDICATOR(object));

   logging_llprintf(LOGLEVEL_DEBUG, "%s: not much to do here?", __func__);
}

gdouble battery_indicator_get_value(BatteryIndicator *batt)
{
   g_return_val_if_fail(BATTERY_IS_INDICATOR(batt), 0.0);
   return batt->value;
}

void battery_indicator_set_value(BatteryIndicator *batt, gdouble new_value)
{
   g_return_if_fail(BATTERY_IS_INDICATOR(batt));
   batt->old_value = batt->value;
   batt->value = new_value;

   g_object_notify_by_pspec(G_OBJECT(batt), battery_indicator_properties[BATTERY_INDICATOR_VALUE_PROP]);
}

