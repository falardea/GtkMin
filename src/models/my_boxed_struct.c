/*
 * Created by french on 7/28/24.
*/
#include "my_boxed_struct.h"

G_DEFINE_BOXED_TYPE(MyBoxedStruct, my_boxed_struct, my_boxed_struct_copy, my_boxed_struct_free);

// MyBoxedStruct *my_boxed_struct_new(void)
// {
//
// }

MyBoxedStruct *my_boxed_struct_copy(MyBoxedStruct *src)
{
   MyBoxedStruct *dest = g_new(MyBoxedStruct, 1);
   *dest = *src;
   return dest;
}

void my_boxed_struct_free(MyBoxedStruct *src)
{
   g_free(src);
}

