/*
 * Copyright (c) 2024 Deka Research and Development. All rights reserved.
 * Created by rfalardeau on 6/25/2024. 
*/
#include <math.h>
#include <glib-object.h>
#include "gtkdial.h"

#define SCROLL_DELAY_LENGTH      300
//#define DIAL_DEFAULT_SIZE        100
static gint DIAL_DEFAULT_SIZE = 100;

/* Forward declarations */
static void       gtk_dial_class_init              (GtkDialClass *klass);
static void       gtk_dial_init                    (GtkDial *dial);
static void       gtk_dial_destroy                 (GObject *object);
static void       gtk_dial_realize                 (GtkWidget *widget);
static void       gtk_dial_size_request            (GtkWidget* widget,
                                                    GtkOrientation orientation,
                                                    gint* minimum_size,
                                                    gint* natural_size);
static void       gtk_dial_size_allocate           (GtkWidget *widget,
                                                    GtkAllocation *allocation);
static gboolean   gtk_dial_draw                    (GtkWidget *widget,
                                                    cairo_t *context);
static gboolean   gtk_dial_button_press            (GtkWidget *widget,
                                                    GdkEventButton *event);
static gboolean   gtk_dial_button_release          (GtkWidget *widget,
                                                    GdkEventButton *event);
static gboolean   gtk_dial_motion_notify           (GtkWidget *widget,
                                                    GdkEventMotion *event);
static gboolean   gtk_dial_timer                   (GtkDial *dial);

static void       gtk_dial_update_mouse            (GtkDial *dial, gint x, gint y);
static void       gtk_dial_update                  (GtkDial *dial);
static void       gtk_dial_adjustment_changed      (GtkAdjustment *adjustment,
                                                    gpointer data);
static void       gtk_dial_adjustment_value_changed(GtkAdjustment *adjustment,
                                                    gpointer data);
/* Local data */
static GtkWidgetClass *parent_class = NULL;

GType gtk_dial_get_type()
{
   static GType dial_type = 0;

   if (!dial_type)
   {
      static const GTypeInfo dial_info =
            {
                  sizeof (GtkDialClass),
                  NULL,
                  NULL,
                  (GClassInitFunc) gtk_dial_class_init,
                  NULL,
                  NULL,
                  sizeof(GtkDial),
                  0,
                  (GInstanceInitFunc) gtk_dial_init,
                  NULL
            };
      dial_type = g_type_register_static(GTK_TYPE_WIDGET, "GtkDial", &dial_info, 0);
   }
   return dial_type;
}

static void gtk_dial_class_init(GtkDialClass *klass)
{
   GObjectClass         *object_class = (GObjectClass *) klass;
   GtkWidgetClass       *widget_class = (GtkWidgetClass *) klass;

   parent_class = g_type_class_ref(gtk_widget_get_type());

   object_class->dispose = gtk_dial_destroy;  // finalize?

   widget_class->realize = gtk_dial_realize;
   widget_class->draw = gtk_dial_draw;
   widget_class->adjust_size_request = gtk_dial_size_request;
   widget_class->size_allocate = gtk_dial_size_allocate;
   widget_class->button_press_event = gtk_dial_button_press;
   widget_class->button_release_event = gtk_dial_button_release;
   widget_class->motion_notify_event = gtk_dial_motion_notify;
}

static void gtk_dial_init(GtkDial *dial)
{
   dial->button = 0;
   dial->policy = GTK_UPDATE_ALWAYS;
   dial->ud_timer = 0;
   dial->radius = 0;
   dial->curr_angle = 0;
   dial->last_angle = 0;
   dial->pointer_width = 0;
   dial->old_value = 0;
   dial->old_lower = 0;
   dial->old_upper = 0;
   dial->adjustment = NULL;
}

GtkWidget* gtk_dial_new(GtkAdjustment *adjustment)
{
   GtkDial *dial;
   dial = g_object_new(gtk_dial_get_type(), NULL);

   if(!adjustment)
   {
      adjustment = (GtkAdjustment*) gtk_adjustment_new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
   }

   gtk_dial_set_adjustment(dial, adjustment);

   g_print("%s\n", __func__);

   return GTK_WIDGET(dial);
}

static void gtk_dial_destroy(GObject *object)
{
   GtkDial *dial;

   g_return_if_fail(object != NULL);
   g_return_if_fail(GTK_IS_DIAL(object));

   dial = GTK_DIAL(object);

   if(dial->adjustment)
   {
      g_object_unref(G_OBJECT(dial->adjustment));
      dial->adjustment = NULL;
   }

   if(G_OBJECT_CLASS(parent_class)->dispose)
   {
      (*G_OBJECT_CLASS(parent_class)->dispose)(object);
   }
   g_print("%s\n", __func__);
}

GtkAdjustment* gtk_dial_get_adjustment(GtkDial *dial)
{
   g_return_val_if_fail(dial != NULL, NULL);
   g_return_val_if_fail(GTK_IS_DIAL(dial), NULL);

   g_print("%s\n", __func__);

   return dial->adjustment;
}

void gtk_dial_update_policy(GtkDial *dial, guint8 policy)
{
   g_return_if_fail(dial != NULL);
   g_return_if_fail(GTK_IS_DIAL(dial));

   dial->policy = policy;
   g_print("%s\n", __func__);
}
void gtk_dial_set_adjustment(GtkDial *dial, GtkAdjustment *adjustment)
{
   g_return_if_fail(dial != NULL);
   g_return_if_fail(GTK_IS_DIAL(dial));

   if(dial->adjustment)
   {
      g_signal_handlers_disconnect_by_func(G_OBJECT(dial->adjustment), NULL, (gpointer) dial);
      g_object_unref(G_OBJECT(dial->adjustment));
   }

   dial->adjustment = adjustment;
   g_object_ref(G_OBJECT(dial->adjustment));
   g_signal_connect(G_OBJECT(dial->adjustment), "changed", G_CALLBACK(gtk_dial_adjustment_changed),
                    (gpointer) dial);
   g_signal_connect(G_OBJECT(dial->adjustment), "value_changed", G_CALLBACK(gtk_dial_adjustment_value_changed),
                    (gpointer) dial);

   dial->old_value = gtk_adjustment_get_value(adjustment);
   dial->old_lower = gtk_adjustment_get_lower(adjustment);
   dial->old_upper = gtk_adjustment_get_upper(adjustment);

   g_print("%s\n", __func__);
   gtk_dial_update(dial);
}

static void gtk_dial_realize(GtkWidget *widget)
{
   GtkDial        *dial;
   GdkWindowAttr  attributes;
   GtkAllocation  allocations;
   gint           attributes_mask;

   g_return_if_fail(widget != NULL);
   g_return_if_fail(GTK_IS_DIAL(widget));

   gtk_widget_set_realized(widget, TRUE);
   dial = GTK_DIAL(widget);

   gtk_widget_get_allocation(widget, &allocations);

   attributes.x            = allocations.x;
   attributes.y            = allocations.y;
   attributes.width        = allocations.width;
   attributes.height       = allocations.height;
   attributes.wclass       = GDK_INPUT_OUTPUT;
   attributes.window_type  = GDK_WINDOW_CHILD;
   attributes.event_mask   = gtk_widget_get_events(widget) |
         GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK |
         GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK |
         GDK_POINTER_MOTION_HINT_MASK;
   // GTK2 has a attributes.colormap, but may have been packed them into visual in GTK3
   attributes.visual       = gtk_widget_get_visual(widget);

   attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL;
   gtk_widget_set_window(widget,gdk_window_new(gtk_widget_get_parent_window(widget), &attributes, attributes_mask));

   gdk_window_set_user_data (gtk_widget_get_window(widget), widget);

   g_print("%s\n", __func__);
   //////////// NOTE ///////////////////
   // The following *may* not be useful
   // Deprecated way of attaching style... "SHOULD" be set on parent
   // widget->style = gtk_style_attach (widget->style, widget->window);
   // cairo_t *cr;
   // GtkStyleContext *style_context;
   // gint width, height;
   // style_context = gtk_widget_get_style_context(widget);
   // width = gtk_widget_get_allocated_width(widget);
   // height = gtk_widget_get_allocated_height(widget);
   // cr = cairo_create(widget->priv);
   // gtk_render_background(style_context, cr, 0, 0, width, height);
}

static void gtk_dial_size_request(GtkWidget* widget,
                                  GtkOrientation orientation,
                                  gint* minimum_size,
                                  gint* natural_size)
{
   minimum_size = &DIAL_DEFAULT_SIZE;
   natural_size = &DIAL_DEFAULT_SIZE;
   g_print("%s\n", __func__);
}

static void gtk_dial_size_allocate(GtkWidget *widget, GtkAllocation *allocation)
{
   GtkDial *dial;

   g_return_if_fail(widget != NULL);
   g_return_if_fail(GTK_IS_DIAL(widget));
   g_return_if_fail(allocation != NULL);

   gtk_widget_set_allocation(widget, allocation);
   dial = GTK_DIAL(widget);

   if (gtk_widget_get_realized(widget))
   {
      gdk_window_move_resize(gtk_widget_get_window(widget), allocation->x, allocation->y,
                             allocation->width, allocation->height);
   }
   dial->radius = MIN (allocation->width, allocation->height) * 0.45;
   dial->pointer_width = dial->radius / 5;
   g_print("%s\n", __func__);
}

static gboolean gtk_dial_draw(GtkWidget *widget,
                              cairo_t *cr)
{
   GtkDial        *dial;
   GdkPoint       points[6];
   gdouble        s, c;
   gdouble        theta, last, increment;
   gdouble        upper, lower;
   GtkStyle       *blank_style;
   gint           xc, yc;
   gint           tick_length;
   gint           i, inc;

   g_return_val_if_fail(widget != NULL, FALSE);
   g_return_val_if_fail(GTK_IS_DIAL(widget), FALSE);
   g_return_val_if_fail(cr != NULL, FALSE);

   dial = GTK_DIAL(widget);

   GtkStyleContext  *context = gtk_widget_get_style_context(widget);
   int width = gtk_widget_get_allocated_width(widget);
   int height = gtk_widget_get_allocated_height(widget);


   gtk_render_background(context, cr, 0, 0, width, height);
   gtk_render_frame(context, cr, 0, 0, width, height);
   gtk_style_context_add_class(gtk_widget_get_style_context(widget), "custom-widget-style");

//   xc = gtk_widget_get_allocated_width(widget) / 2;
//   yc = gtk_widget_get_allocated_height(widget) / 2;
//
//   upper = gtk_adjustment_get_upper(dial->adjustment);
//   lower = gtk_adjustment_get_lower(dial->adjustment);
//
//   s = sin(dial->last_angle);
//   c = cos(dial->last_angle);
//   dial->last_angle = dial->curr_angle;
//
//   points[0].x = xc + s * dial->pointer_width/2;
//   points[0].y = yc + c * dial->pointer_width/2;
//   points[1].x = xc + c * dial->radius;
//   points[1].y = yc - s * dial->radius;
//   points[2].x = xc - s * dial->pointer_width/2;
//   points[2].y = yc - c * dial->pointer_width/2;
//   points[3].x = xc - c * dial->radius/10;
//   points[3].y = yc + s * dial->radius/10;
//   points[4].x = points[0].x;
//   points[4].y = points[0].y;
   g_print("%s\n", __func__);
   return FALSE;
}

static gboolean gtk_dial_button_press(GtkWidget *widget, GdkEventButton *event)
{
   GtkDial        *dial;
   gint           dx, dy;
   gdouble        s, c;
   gdouble        d_parallel;
   gdouble        d_perpendicular;

   g_return_val_if_fail(widget != NULL, FALSE);
   g_return_val_if_fail(GTK_IS_DIAL(widget), FALSE);
   g_return_val_if_fail(event != NULL, FALSE);

   dial = GTK_DIAL(widget);

   g_print("%s\n", __func__);

   return FALSE;
}

static gboolean gtk_dial_button_release(GtkWidget *widget, GdkEventButton *event)
{
   GtkDial        *dial;

   g_return_val_if_fail(widget != NULL, FALSE);
   g_return_val_if_fail(GTK_IS_DIAL(widget), FALSE);
   g_return_val_if_fail(event != NULL, FALSE);

   dial = GTK_DIAL(widget);

   g_print("%s\n", __func__);

   return FALSE;
}

static gboolean gtk_dial_motion_notify(GtkWidget *widget, GdkEventMotion *event)
{
   GtkDial           *dial;
   GdkModifierType   mods;
   gint              x, y, mask;

   g_return_val_if_fail(widget != NULL, FALSE);
   g_return_val_if_fail(GTK_IS_DIAL(widget), FALSE);
   g_return_val_if_fail(event != NULL, FALSE);

   dial = GTK_DIAL(widget);

   g_print("%s\n", __func__);

   return FALSE;
}

static gboolean gtk_dial_timer(GtkDial *dial)
{
   g_return_val_if_fail(dial != NULL, FALSE);
   g_return_val_if_fail(GTK_IS_DIAL(dial), FALSE);

   g_print("%s\n", __func__);

   return FALSE;
}

static void gtk_dial_update_mouse(GtkDial *dial, gint x, gint y)
{
   gint     xc, yc;
   gdouble  old_value;

   g_return_if_fail(dial != NULL);
   g_return_if_fail(GTK_IS_DIAL(dial));

   g_print("%s\n", __func__);
}

static void gtk_dial_update(GtkDial *dial)
{
   g_return_if_fail(dial != NULL);
   g_return_if_fail(GTK_IS_DIAL(dial));

   g_print("%s\n", __func__);
}

static void gtk_dial_adjustment_changed(GtkAdjustment *adjustment, gpointer data)
{
   GtkDial        *dial;

   g_return_if_fail(adjustment != NULL);
   g_return_if_fail(data != NULL);

   dial = GTK_DIAL(data);

   g_print("%s\n", __func__);
}

static void gtk_dial_adjustment_value_changed(GtkAdjustment *adjustment, gpointer data)
{
   GtkDial *dial;

   g_return_if_fail(adjustment != NULL);
   g_return_if_fail(data != NULL);

   dial = GTK_DIAL(data);

   g_print("%s\n", __func__);
}