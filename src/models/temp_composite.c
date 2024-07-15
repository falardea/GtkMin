/*
 * Copyright (c) 2024
 * Created by rfalardeau on 7/15/2024. 
*/

#include "temp_composite.h"
#include "utils/logging.h"

struct _TempComposite
{
   GtkBox   parent;
   GtkLabel *name_label;

   gchar    *name;

//   gboolean (* btn_press_event) (GtkWidget *widget, GdkEventButton *event, gpointer user_data);
};

enum {
   PROP_0, // reserved
   TEMP_COMPOSITE_PROP_NAME,
   N_TEMP_COMPOSITE_PROPERTIES
};

G_DEFINE_TYPE(TempComposite, temp_composite, GTK_TYPE_BOX)

static void temp_composite_finalize          ( GObject *object );
//static void temp_composite_realize           (GtkWidget *widget);

static void temp_composite_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
   TempComposite *self = TEMP_COMPOSITE(object);
   switch(prop_id){
      case TEMP_COMPOSITE_PROP_NAME:
         temp_composite_set_name(self, g_value_get_string( value ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void temp_composite_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
   TempComposite *self = TEMP_COMPOSITE(object);
   switch(prop_id)
   {
      case TEMP_COMPOSITE_PROP_NAME:
         g_value_set_string( value, temp_composite_get_name(self));
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static GParamSpec *temp_composite_properties[N_TEMP_COMPOSITE_PROPERTIES] = {NULL, };




gboolean btn_press_event(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   return FALSE;
}

//static GtkWidgetClass *parent_class = NULL;
static void temp_composite_class_init(TempCompositeClass *klass)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   GObjectClass *o_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *w_class = (GtkWidgetClass *) klass;

//   parent_class = g_type_class_ref(gtk_widget_get_type());

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS (klass), "/mini_app/resources/temp_composite.glade");
   gtk_widget_class_bind_template_child(w_class, TempComposite , name_label);
   gtk_widget_class_bind_template_callback_full(w_class, "btn_press_event", (GCallback)btn_press_event);

//   w_class->realize = temp_composite_realize;

   o_class->finalize = temp_composite_finalize;
   o_class->set_property = temp_composite_set_property;
   o_class->get_property = temp_composite_get_property;

   temp_composite_properties[TEMP_COMPOSITE_PROP_NAME] = g_param_spec_string("name", NULL, NULL, NULL, G_PARAM_READWRITE );
   g_object_class_install_properties( o_class, N_TEMP_COMPOSITE_PROPERTIES, temp_composite_properties);
}

static void temp_composite_init(TempComposite *self)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
//   gtk_widget_add_events(GTK_WIDGET(self), GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK );
   gtk_widget_init_template(GTK_WIDGET(self));
}

static void temp_composite_finalize(GObject *self)
{
   TempComposite *comp = TEMP_COMPOSITE(self);
   g_free(comp->name);
   GObjectClass *parent_class = G_OBJECT_CLASS( temp_composite_parent_class );
   logging_llprintf(LOGLEVEL_DEBUG, "object instance finalize");
   ( *parent_class->finalize )( self );
}

GtkWidget *temp_composite_new(const gchar *new_name)
{
   TempComposite *comp;
   comp = g_object_new(temp_composite_get_type(), NULL);
   if (new_name != NULL)
   {
      temp_composite_set_name(comp, new_name);
   }

   return GTK_WIDGET(comp);
}

gchar *temp_composite_get_name( TempComposite *self )
{
   g_return_val_if_fail(self != NULL, NULL);
   g_return_val_if_fail( TEMP_IS_COMPOSITE( self ), NULL );

   return g_strdup( self->name );
}
void temp_composite_set_name( TempComposite *self, const gchar *name )
{
   g_return_if_fail( name != NULL);
   g_return_if_fail( TEMP_IS_COMPOSITE( self ) );

   if( self->name != NULL )
      g_free( self->name );

   self->name = g_strdup( name );
   gtk_label_set_label(self->name_label, temp_composite_get_name(self));
}


//static void temp_composite_realize(GtkWidget *widget)
//{
//   GdkWindowAttr  win_attr;
//   GtkAllocation  alloc;
//   gint           attr_mask;
//
//   g_return_if_fail(widget != NULL);
//   g_return_if_fail(TEMP_IS_COMPOSITE(widget));
//
//   gtk_widget_set_realized(widget, TRUE);
//   gtk_widget_get_allocation(widget, &alloc);
//   win_attr.x = alloc.x;
//   win_attr.y = alloc.y;
//   win_attr.width = alloc.width;
//   win_attr.height = alloc.height;
//   win_attr.wclass = GDK_INPUT_OUTPUT;
//   win_attr.window_type = GDK_WINDOW_CHILD;
//   win_attr.event_mask = gtk_widget_get_events(widget) | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK;
//   win_attr.visual = gtk_widget_get_visual(widget);
//   attr_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL;
//   gtk_widget_set_window(widget, gdk_window_new(gtk_widget_get_parent_window(widget), &win_attr, attr_mask));
//   gdk_window_set_user_data(gtk_widget_get_window(widget), widget);
//}