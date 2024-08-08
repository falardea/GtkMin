/*
 * Created by french on 7/28/24.
*/
#include "my_int.h"

MyInt *my_int_new(void)
{
   MyInt *dest = g_new(MyInt, 1);
   return dest;
}

MyInt *my_int_copy(MyInt *src)
{
   MyInt *dest = g_new(MyInt, 1);
   *dest = *src;
   return dest;
}

void my_int_free(MyInt *src)
{
   g_free(src);
}

G_DEFINE_BOXED_TYPE(MyInt, my_int, my_int_copy, my_int_free);