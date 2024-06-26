/*
 * Created by french on 3/2/24.
 *
 * This is a file we're using to explore the creation and usage of custom-made GObjects.
 *
 * Primary source for example:
 * https://ewgeny.wordpress.com/2012/02/03/deriving-a-custom-class-from-gobject/
 * Also:
 * obviously: https://docs.gtk.org/gobject/tutorial.html
 * and: https://github.com/clobrano/gobject-class-creation-example/blob/master/example-person.h
*/
#ifndef GTKSANDBOX_MODEL_DEVICE_H
#define GTKSANDBOX_MODEL_DEVICE_H
#include <glib-object.h>

#ifdef __cplusplus
extern "C" {
#endif

/* include any other dependencies for this object here - ie, different types */
G_BEGIN_DECLS

/* our GObject declaration */

#define MODEL_TYPE_DEVICE              ( model_device_get_type( ) )

G_DECLARE_FINAL_TYPE(ModelDevice, model_device, MODEL, DEVICE, GObject)

/* Methods for our newly declared GObject derived GType*/
ModelDevice *model_device_new( const gchar *name );

gchar *model_device_get_name( ModelDevice *self );
void model_device_set_name( ModelDevice *self, const gchar *name );

gboolean model_device_get_enabled( ModelDevice *self );
void model_device_set_enabled( ModelDevice *self, gboolean enabled );

G_END_DECLS

#ifdef __cplusplus
}  /* closing brace for extern "C" */
#endif
#endif  /* GTKSANDBOX_MODEL_DEVICE_H */
