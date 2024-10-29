#include "printing.h"

#include <stdio.h>

#include "stdlib.h"
#include "storage.h"

void dict_print_indent(Dict *d, int indent, char fromDict);
void dict_print(Dict *d);

void arr_print_indent(Array *a, int indent, char fromDict)
{
    if (!a)
    {
        return;
    }

    // Obtains the number of tab characters that will be printed
    char *tabs = calloc(indent, sizeof(char));
    if (!tabs)
    {
        return;
    }
    for (int i = 0; i < indent - 1; ++i)
    {
        tabs[i] = '\t';
    }
    tabs[indent - 1] = '\0';

    uint_fast64_t size = a->size;
    // Empty array
    if (size == 0)
    {
        printf("%s[]", fromDict ? "" : tabs);
        free(tabs);
        return;
    }

    printf("%s[\n", fromDict ? "" : tabs);

    for (uint_fast64_t i = 0; i < size; ++i)
    {
        Value v = array_get(a, i);
        if (v.type == T_ERROR)
        {
            continue;
        }

        switch (v.type)
        {
        case T_STR:
            printf("\t%s\"%s\"", tabs, v.strv.str ? v.strv.str : "");
            break;
        case T_INT:
            printf("\t%s%d", tabs, v.intv);
            break;
        case T_DOUBLE:
            printf("\t%s%f", tabs, v.doublev);
            break;
        case T_BOOL:
            printf("\t%s%s", tabs, v.boolv ? "true" : "false");
            break;
        case T_NULL:
            printf("\t%snull", tabs);
            break;
        case T_ARR:
            arr_print_indent(v.arrayv, indent + 1, 0);
            break;
        case T_DICT:
            dict_print_indent(v.dictv, indent + 1, 0);
            break;
        }

        if (i < size - 1)
        {
            printf(",\n");
        }
    }

    printf("\n%s]", tabs);
    free(tabs);
}

void arr_print(Array *a)
{
    if (a)
    {
#ifndef VALGRING_DISABLE_PRINT
        arr_print_indent(a, 1, 0);
#endif
    }
}

void dict_print_indent(Dict *d, int indent, char fromDict)
{
    // Obtains the number of tab characters that will be printed
    char *tabs = calloc(indent, sizeof(char));
    if (!tabs)
    {
        return;
    }
    for (int i = 0; i < indent - 1; ++i)
    {
        tabs[i] = '\t';
    }
    tabs[indent - 1] = '\0';

    int size = d->size;
    if (size == 0)
    {
        printf("%s{}", fromDict ? "" : tabs);
        free(tabs);
        return;
    }

    printf("%s{\n", fromDict ? "" : tabs);

    int i = 0;
    ItemLink *link = d->head;
    while (link)
    {
        String key = link->key;
        Item it = dict_get(d, key);
        if (it.type == T_ERROR)
        {
            continue;
        }

        switch (it.type)
        {
        case T_STR:
            printf("\t%s\"%s\" : \"%s\"", tabs, key.str,
                   it.strv.str ? it.strv.str : "");
            break;
        case T_INT:
            printf("\t%s\"%s\" : %d", tabs, key.str, it.intv);
            break;
        case T_DOUBLE:
            printf("\t%s\"%s\" : %f", tabs, key.str, it.doublev);
            break;
        case T_BOOL:
            printf("\t%s\"%s\" : %s", tabs, key.str,
                   it.boolv ? "true" : "false");
            break;
        case T_NULL:
            printf("\t%s\"%s\" : null", tabs, key.str);
            break;
        case T_ARR:
            printf("\t%s\"%s\" : ", tabs, key.str);
            arr_print_indent(it.arrayv, indent + 1, 1);
            break;
        case T_DICT:
            printf("\t%s\"%s\" : ", tabs, key.str);
            dict_print_indent(it.dictv, indent + 1, 1);
            break;
        }

        if (i < size - 1)
        {
            printf(",\n");
        }
        ++i;
        link = link->next;
    }

    printf("\n%s}", tabs);
    free(tabs);
}

void dict_print(Dict *d)
{
    if (d)
    {
#ifndef VALGRING_DISABLE_PRINT
        dict_print_indent(d, 1, 0);
#endif
    }
}
