/*
 * Created by french on 3/2/24.
*/
#include <glib-object.h>
#include "../utils/logging.h"
#include "model_device.h"

struct _ModelDevice
{
   GObject  parent_object;
   gboolean enabled;
   gchar    *device_name;
};

G_DEFINE_TYPE( ModelDevice, model_device, G_TYPE_OBJECT )

enum
{
   PROP_0, // Reserved for GObject
   PROP_ENABLED,
   PROP_DEVICE_NAME,
   N_PROPERTIES
};

static void model_device_finalize( GObject *self );

static void model_device_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec )
{
   ModelDevice *device = MODEL_DEVICE( object );

   switch( prop_id ) {
      case PROP_DEVICE_NAME:
         model_device_set_name( device, g_value_get_string( value ) );
         break;
      case PROP_ENABLED:
         model_device_set_enabled(device, g_value_get_boolean( value ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void model_device_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec )
{
   ModelDevice *device = MODEL_DEVICE( object );

   switch( prop_id ) {
      case PROP_DEVICE_NAME:
         g_value_set_string( value, model_device_get_name( device ) );
         break;
      case PROP_ENABLED:
         g_value_set_boolean( value, model_device_get_enabled( device ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static GParamSpec *device_properties[N_PROPERTIES] = {NULL, };

static void model_device_class_init( ModelDeviceClass *klass )
{
   GObjectClass *gobject_class = G_OBJECT_CLASS( klass );

   gobject_class->finalize = model_device_finalize;

   gobject_class->get_property = model_device_get_property;
   gobject_class->set_property = model_device_set_property;

   device_properties[PROP_ENABLED] = g_param_spec_boolean("device-enabled",
                                                          "Device Enabled",
                                                          "Device's enabled state",
                                                          FALSE,
                                                          G_PARAM_READWRITE );
   device_properties[PROP_DEVICE_NAME] = g_param_spec_string("device-name",
                                                             "DeviceName",
                                                             "Device's Name",
                                                             NULL,
                                                             G_PARAM_READWRITE );
   g_object_class_install_properties( gobject_class, N_PROPERTIES, device_properties);
}

/////////////////// INSTANCE //////////////////////////////
static void model_device_init( ModelDevice *self )
{
   self->device_name = g_strdup( "INIT_UNNAMED" );
   self->enabled = FALSE;
   logging_llprintf(LOGLEVEL_DEBUG, "object instance init: device_name = %s", self->device_name);
}

static void model_device_finalize( GObject *self )
{
   ModelDevice *thisDevice = MODEL_DEVICE(self );
   g_free(thisDevice->device_name);
   GObjectClass *parent_class = G_OBJECT_CLASS( model_device_parent_class );
   logging_llprintf(LOGLEVEL_DEBUG, "object instance finalize");
   ( *parent_class->finalize )( self );
}

ModelDevice *model_device_new( const gchar *name )
{
   ModelDevice *device;

   device = MODEL_DEVICE( g_object_new( MODEL_TYPE_DEVICE, NULL ) );

   if( name != NULL )
      model_device_set_name( device, name );

   return device;
}

gboolean model_device_get_enabled( ModelDevice *self )
{
   g_return_val_if_fail( MODEL_IS_DEVICE( self ), FALSE );
   return self->enabled;
}
void model_device_set_enabled( ModelDevice *self, gboolean enabled )
{
   g_return_if_fail( MODEL_IS_DEVICE( self ) );
   self->enabled = enabled;
   g_object_notify_by_pspec(G_OBJECT(self), device_properties[PROP_ENABLED]);
}

gchar *model_device_get_name( ModelDevice *self )
{
   g_return_val_if_fail( MODEL_IS_DEVICE( self ), NULL );

   return g_strdup( self->device_name );
}
void model_device_set_name( ModelDevice *self, const gchar *name )
{
   g_return_if_fail( name );
   g_return_if_fail( MODEL_IS_DEVICE( self ) );

   if( self->device_name != NULL )
      g_free( self->device_name );

   self->device_name = g_strdup( name );
}

