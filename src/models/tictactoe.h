/*
 * Created by rfalardeau on 6/25/2024.
*/

#ifndef GTKMIN_TICTACTOE_H_
#define GTKMIN_TICTACTOE_H_
#include <glib-object.h>
#include <gtk/gtk.h>

/**
 * To explore Gtk Composite structures, we are going through this tutorial:
 * https://www.geany.org/manual/gtk/gtk-tutorial/x2202.html
 */
G_BEGIN_DECLS // C++ guard, from gmacros.h

#define TICTACTOE_TYPE              (tictactoe_get_type ())
#define TICTACTOE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), tictactoe_get_type(), Tictactoe))
#define TICTACTOE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TICTACTOE_TYPE, TictactoeClass))

typedef struct _Tictactoe           Tictactoe;
typedef struct _TictactoeClass      TictactoeClass;

struct _Tictactoe
{
   GtkGrid gbox;
   GtkWidget *buttons[3][3];
};

struct _TictactoeClass
{
   GtkGridClass parent_class;

   void (*tictactoe) (Tictactoe *ttt);
};

GType          tictactoe_get_type         (void);
GtkWidget*     tictactoe_new              (void);
void           tictactoe_clear            (Tictactoe *ttt);

G_END_DECLS
#endif  // GTKMIN_TICTACTOE_H_
