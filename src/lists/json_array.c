#include "json_array.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
json_array_st *array_init(size_t size)
{
    json_array_st *ja = calloc(1, sizeof(json_array_st));
    if (ja == NULL)
    {
        return NULL;
    }
    ja->size = size;
    ja->elts = calloc(size, sizeof(struct array_elt));
    if (ja->elts == NULL)
    {
        free(ja);
        return NULL;
    }
    return ja;
}

void array_append(json_array_st *l, struct array_elt elt)
{
    if (l == NULL || l->idx >= l->size - 1)
    {
        return;
    }
    l->elts[l->idx++] = elt;
}

typed_value_st array_get(json_array_st *ja, size_t index)
{
    if (ja == NULL || index >= ja->size)
    {
        return (typed_value_st){ .type = TYPE_ERROR, .value = NULL };
    }
    return (typed_value_st){ .type = ja->elts[index].type,
                             .value = ja->elts[index].value };
}

void array_print(json_array_st *l)
{
    array_print_indent(l, 1, 0);
    printf("\n");
}

void array_print_indent(json_array_st *l, char indent, char from_list)
{
    char *tabs = calloc(indent, sizeof(char));
    if (tabs == NULL)
    {
        return;
    }
    for (int i = 0; i < indent - 1; ++i)
    {
        tabs[i] = '\t';
    }
    tabs[indent - 1] = '\0';

    if (l == NULL || l->elts == NULL || l->size == 0)
    {
        printf("%s[]", from_list ? tabs : "");
    }
    else
    {
        printf("%s[\n", from_list ? tabs : "");

        for (size_t i = 0; i < l->size; ++i)
        {
            switch (l->elts[i].type)
            {
            case TYPE_STR:
                printf("%s\t\"%s\"", tabs, (char *)l->elts[i].value);
                break;
            case TYPE_NUM:
                printf("%s\t%lu", tabs, *(long *)l->elts[i].value);
                break;
            case TYPE_ARR:
                array_print_indent(l->elts[i].value, indent + 1, 1);
                break;
            case TYPE_BOOL:
                printf("%s\t%s", tabs,
                       *(char *)l->elts[i].value ? "true" : "false");
                break;
            case TYPE_NULL:
                printf("%s\tnull", tabs);
                break;
            default:
                break;
            }

            if (i != l->size - 1)
            {
                printf(",\n");
            }
        }
        printf("\n%s]", tabs);
    }
    free(tabs);
}
