#include <stdio.h>

#include "storage.h"

int main(int argc, char *argv[])
{
    if (argc != 2 || !argv)
    {
        return 1;
    }

    Array a = init_array(5);
    arr_add_int(&a, 1);
    arr_add_int(&a, 2);
    arr_add_int(&a, 3);
    arr_add_int(&a, 4);
    arr_add_int(&a, 5);
    for (int i = 0; i < a.size; ++i)
    {
        ArrayLink al = array_get(&a, i);
        switch (al.type)
        {
        case T_STR:
            printf("%s\n", al.str.str ? al.str.str : "");
            break;
        case T_INT:
            printf("%d\n", al.integer);
            break;
        }
    }
    destroy_array(&a);
    return 0;
}
