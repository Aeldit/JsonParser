#include <stdio.h>

#include "storage.h"

void dict_print(Dict *d);

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
        case T_DICT:
            dict_print(al.dictv);
            break;
        }
    }
}

void dict_print(Dict *d)
{
    int size = d->size;
    for (int i = 0; i < size; ++i)
    {
        String key = d->keys_types[i].key;
        DictLink dl = dict_get(d, key);
        switch (dl.type)
        {
        case T_STR:
            printf("%s : %s\n", key.str, dl.strv.str ? dl.strv.str : "");
            break;
        case T_INT:
            printf("%s : %d\n", key.str, dl.integerv);
            break;
        case T_DOUBLE:
            printf("%s : %f\n", key.str, dl.doublev);
            break;
        case T_BOOL:
            printf("%s : %s\n", key.str, dl.boolv ? "true" : "false");
            break;
        case T_NULL:
            printf("%s : null\n", key.str);
            break;
        case T_ARR:
            printf("%s : ", key.str);
            arr_print(dl.arrayv);
            break;
        case T_DICT:
            printf("%s : ", key.str);
            dict_print(dl.dictv);
            break;
        }
    }
}
