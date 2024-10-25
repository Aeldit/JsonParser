#include <stdio.h>

#include "storage.h"

void arr_print(Array *a)
{
    int size = a->size;
    for (int i = 0; i < size; ++i)
    {
        ArrayLink al = array_get(a, i);
        switch (al.type)
        {
        case T_STR:
            printf("%s\n", al.strv.str ? al.strv.str : "");
            break;
        case T_INT:
            printf("%d\n", al.integerv);
            break;
        case T_DOUBLE:
            printf("%f\n", al.doublev);
            break;
        case T_BOOL:
            printf("%s\n", al.boolv ? "true" : "false");
            break;
        case T_NULL:
            printf("null\n");
            break;
        case T_ARR:
            arr_print(al.arrayv);
            break;
        }
    }
}
