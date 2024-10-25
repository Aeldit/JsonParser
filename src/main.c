#include <stdio.h>

#include "storage.h"

int main(int argc, char *argv[])
{
    if (argc != 2 || !argv)
    {
        return 1;
    }

    Array a = init_array(12);

    String s1 = { .str = "Test", .length = 4 };
    String s2 = { .str = "Hello", .length = 5 };

    arr_add_int(&a, 1);
    arr_add_int(&a, 2);
    arr_add_str(&a, s1);
    arr_add_str(&a, s2);
    arr_add_int(&a, 5);
    arr_add_null(&a);
    arr_add_null(&a);
    arr_add_bool(&a, 1);
    arr_add_bool(&a, 0);
    arr_add_double(&a, 0.5);
    arr_add_double(&a, 55.789);

    for (int i = 0; i < a.size; ++i)
    {
        ArrayLink al = array_get(&a, i);
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
            break;
        }
    }

    destroy_array(&a);
    return 0;
}
