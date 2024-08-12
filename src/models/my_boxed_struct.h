/*
 * Created by french on 7/28/24.
*/
#ifndef MY_BOXED_STRUCT_H_
#define MY_BOXED_STRUCT_H_
#include <gtk/gtk.h>
G_BEGIN_DECLS

#define MY_BOXED_STRUCT_TYPE ( my_boxed_struct_get_type() )

typedef struct MyBoxedStruct
{
   long value;
} MyBoxedStruct;

GType my_boxed_struct_get_type(void) G_GNUC_CONST;

// MyBoxedStruct *my_boxed_struct_new(void);

MyBoxedStruct *my_boxed_struct_copy(MyBoxedStruct *src);

void my_boxed_struct_free(MyBoxedStruct *src);

G_END_DECLS
#endif  // MY_BOXED_STRUCT_H_
