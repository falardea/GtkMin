/*
 * Created by french on 7/28/24.
*/
#ifndef MY_INT_H_
#define MY_INT_H_
#include <gtk/gtk.h>
G_BEGIN_DECLS

typedef struct
{
   long value;
} MyInt;

#define MY_INT_STRUCT my_int_get_type()
GType my_int_get_type(void) G_GNUC_CONST;

MyInt *my_int_new(void);

MyInt *my_int_copy(MyInt *src);

void my_int_free(MyInt *src);



G_END_DECLS
#endif  // MY_INT_H_
