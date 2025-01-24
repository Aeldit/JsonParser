#include "ro_json_storage.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define RO_VALUE_OF(T_TYPE, type_field)                                        \
    ((ro_value_t){ .type = T_TYPE, .type_field = value })
#define RO_ITEM_OF(T_TYPE, type_field)                                         \
    ((ro_item_t){ .type = T_TYPE, .key = key, .type_field = value })

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
ro_array_t *init_ro_array(unsigned size)
{
    ro_array_t *a = calloc(1, sizeof(ro_array_t));
    if (!a)
    {
        return 0;
    }

    if (!size)
    {
        return a;
    }

    a->values = calloc(size, sizeof(ro_value_t));
    if (!a->values)
    {
        free(a);
        return 0;
    }
    a->size = size;
    return a;
}

ro_dict_t *init_ro_dict(unsigned size)
{
    ro_dict_t *d = calloc(1, sizeof(ro_dict_t));
    if (!d)
    {
        return 0;
    }

    if (!size)
    {
        return d;
    }

    d->items = calloc(size, sizeof(ro_item_t));
    if (!d->items)
    {
        free(d);
        return 0;
    }
    d->size = size;
    return d;
}

ro_json_t *init_ro_json(char is_array, ro_array_t *a, ro_dict_t *d)
{
    if ((is_array && !a) || (!is_array && !d))
    {
        return 0;
    }

    ro_json_t *j = calloc(1, sizeof(ro_json_t));
    if (!j)
    {
        return 0;
    }
    j->is_array = is_array;
    if (is_array)
    {
        j->array = a;
    }
    else
    {
        j->dict = d;
    }
    return j;
}

/*******************************************************************************
**                                     ADDS                                   **
*******************************************************************************/
void ro_array_add_str(ro_array_t *a, string_t value)
{
    if (a && a->values && a->insert_index < a->size && value.str)
    {
        a->values[a->insert_index++] = RO_VALUE_OF(T_STR, strv);
    }
}

void ro_array_add_long(ro_array_t *a, long value)
{
    if (a && a->values && a->insert_index < a->size)
    {
        a->values[a->insert_index++] = RO_VALUE_OF(T_LONG, longv);
    }
}

void ro_array_add_double(ro_array_t *a, double value)
{
    if (a && a->values && a->insert_index < a->size)
    {
        a->values[a->insert_index++] = RO_VALUE_OF(T_DOUBLE, doublev);
    }
}

void ro_array_add_exp_long(ro_array_t *a, exponent_long_t value)
{
    if (a && a->values && a->insert_index < a->size)
    {
        a->values[a->insert_index++] = RO_VALUE_OF(T_EXP_LONG, exp_longv);
    }
}

void ro_array_add_exp_double(ro_array_t *a, exponent_double_t value)
{
    if (a && a->values && a->insert_index < a->size)
    {
        a->values[a->insert_index++] = RO_VALUE_OF(T_EXP_DOUBLE, exp_doublev);
    }
}

void ro_array_add_bool(ro_array_t *a, char value)
{
    if (a && a->values && a->insert_index < a->size)
    {
        a->values[a->insert_index++] = RO_VALUE_OF(T_BOOL, boolv);
    }
}

void ro_array_add_null(ro_array_t *a)
{
    if (a && a->values && a->insert_index < a->size)
    {
        a->values[a->insert_index++] = (ro_value_t){ .type = T_NULL };
    }
}

void ro_array_add_array(ro_array_t *a, ro_array_t *value)
{
    if (a && a->values && value && a->insert_index < a->size)
    {
        a->values[a->insert_index++] = RO_VALUE_OF(T_ARR, arrayv);
    }
}

void ro_array_add_dict(ro_array_t *a, ro_dict_t *value)
{
    if (a && a->values && value && a->insert_index < a->size)
    {
        a->values[a->insert_index++] = RO_VALUE_OF(T_DICT, dictv);
    }
}

void ro_dict_add_str(ro_dict_t *d, string_t key, string_t value)
{
    if (d && d->items && d->insert_index < d->size && key.str && value.str)
    {
        d->items[d->insert_index++] = RO_ITEM_OF(T_STR, strv);
    }
}

void ro_dict_add_long(ro_dict_t *d, string_t key, long value)
{
    if (d && d->items && d->insert_index < d->size && key.str)
    {
        d->items[d->insert_index++] = RO_ITEM_OF(T_LONG, longv);
    }
}

void ro_dict_add_double(ro_dict_t *d, string_t key, double value)
{
    if (d && d->items && d->insert_index < d->size && key.str)
    {
        d->items[d->insert_index++] = RO_ITEM_OF(T_DOUBLE, doublev);
    }
}

void ro_dict_add_exp_long(ro_dict_t *d, string_t key, exponent_long_t value)
{
    if (d && d->items && d->insert_index < d->size && key.str)
    {
        d->items[d->insert_index++] = RO_ITEM_OF(T_EXP_LONG, exp_longv);
    }
}

void ro_dict_add_exp_double(ro_dict_t *d, string_t key, exponent_double_t value)
{
    if (d && d->items && d->insert_index < d->size && key.str)
    {
        d->items[d->insert_index++] = RO_ITEM_OF(T_EXP_DOUBLE, exp_doublev);
    }
}

void ro_dict_add_bool(ro_dict_t *d, string_t key, char value)
{
    if (d && d->items && d->insert_index < d->size && key.str)
    {
        d->items[d->insert_index++] = RO_ITEM_OF(T_BOOL, boolv);
    }
}

void ro_dict_add_null(ro_dict_t *d, string_t key)
{
    if (d && d->items && d->insert_index < d->size && key.str)
    {
        d->items[d->insert_index++] = (ro_item_t){ .type = T_NULL, .key = key };
    }
}

void ro_dict_add_array(ro_dict_t *d, string_t key, ro_array_t *value)
{
    if (d && d->items && value && d->insert_index < d->size && key.str)
    {
        d->items[d->insert_index++] = RO_ITEM_OF(T_ARR, arrayv);
    }
}

void ro_dict_add_dict(ro_dict_t *d, string_t key, ro_dict_t *value)
{
    if (d && d->items && value && d->insert_index < d->size && key.str)
    {
        d->items[d->insert_index++] = RO_ITEM_OF(T_DICT, dictv);
    }
}

/*******************************************************************************
**                                    GETS                                    **
*******************************************************************************/
ro_value_t ro_array_get(ro_array_t *a, unsigned index)
{
    if (!a || index >= a->size)
    {
        return ERROR_RO_VALUE;
    }

    return index < a->size ? a->values[index] : ERROR_RO_VALUE;
}

ro_item_t ro_dict_get(ro_dict_t *d, string_t key)
{
    if (!d || !key.str)
    {
        return ERROR_RO_ITEM;
    }

    ro_item_t *items = d->items;
    unsigned size = d->size;
    for (unsigned i = 0; i < size; ++i)
    {
        ro_item_t it = items[i];
        if (strings_equals(key, it.key))
        {
            return it;
        }
    }
    return ERROR_RO_ITEM;
}

/*******************************************************************************
**                                   PRINTING                                 **
*******************************************************************************/
void ro_dict_print_indent(ro_dict_t *d, unsigned indent, char fromDict);
void ro_dict_print(ro_dict_t *d);

void ro_array_print_indent(ro_array_t *a, unsigned indent, char fromDict)
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
    for (unsigned i = 0; i < indent - 1; ++i)
    {
        tabs[i] = '\t';
    }
    tabs[indent - 1] = '\0';

    unsigned size = a->size;
    // Empty array
    if (size == 0)
    {
        printf("%s[]", fromDict ? "" : tabs);
        free(tabs);
        return;
    }

    printf("%s[\n", fromDict ? "" : tabs);

    ro_value_t *values = a->values;
    for (unsigned i = 0; i < size; ++i)
    {
        ro_value_t v = values[i];
        if (v.type == T_ERROR)
        {
            continue;
        }

        switch (v.type)
        {
        case T_STR:
            printf("\t%s\"%s\"", tabs, v.strv.str ? v.strv.str : "");
            break;
        case T_LONG:
            printf("\t%s%ld", tabs, v.longv);
            break;
        case T_DOUBLE:
            printf("\t%s%f", tabs, v.doublev);
            break;
        case T_EXP_LONG:
            printf("\t%s%lde%ld", tabs, v.exp_longv.number,
                   v.exp_longv.exponent);
            break;
        case T_EXP_DOUBLE:
            printf("\t%s%fe%ld", tabs, v.exp_doublev.number,
                   v.exp_doublev.exponent);
            break;
        case T_BOOL:
            printf("\t%s%s", tabs, v.boolv ? "true" : "false");
            break;
        case T_NULL:
            printf("\t%snull", tabs);
            break;
        case T_ARR:
            ro_array_print_indent(v.arrayv, indent + 1, 0);
            break;
        case T_DICT:
            ro_dict_print_indent(v.dictv, indent + 1, 0);
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

void ro_array_print(ro_array_t *a)
{
    if (a)
    {
#ifndef VALGRING_DISABLE_PRINT
        ro_array_print_indent(a, 1, 0);
#endif
    }
}

void ro_dict_print_indent(ro_dict_t *d, unsigned indent, char fromDict)
{
    // Obtains the number of tab characters that will be printed
    char *tabs = calloc(indent, sizeof(char));
    if (!tabs)
    {
        return;
    }
    for (unsigned i = 0; i < indent - 1; ++i)
    {
        tabs[i] = '\t';
    }
    tabs[indent - 1] = '\0';

    unsigned size = d->size;
    if (size == 0)
    {
        printf("%s{}", fromDict ? "" : tabs);
        free(tabs);
        return;
    }

    printf("%s{\n", fromDict ? "" : tabs);

    ro_item_t *items = d->items;
    for (unsigned i = 0; i < size; ++i)
    {
        ro_item_t it = items[i];
        if (it.type == T_ERROR)
        {
            continue;
        }

        string_t key = it.key;
        switch (it.type)
        {
        case T_STR:
            printf("\t%s\"%s\" : \"%s\"", tabs, key.str,
                   it.strv.str ? it.strv.str : "");
            break;
        case T_LONG:
            printf("\t%s\"%s\" : %ld", tabs, key.str, it.longv);
            break;
        case T_DOUBLE:
            printf("\t%s\"%s\" : %f", tabs, key.str, it.doublev);
            break;
        case T_EXP_LONG:
            printf("\t%s\"%s\" : %lde%ld", tabs, key.str, it.exp_longv.number,
                   it.exp_longv.exponent);
            break;
        case T_EXP_DOUBLE:
            printf("\t%s\"%s\" : %fe%ld", tabs, key.str, it.exp_doublev.number,
                   it.exp_doublev.exponent);
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
            ro_array_print_indent(it.arrayv, indent + 1, 1);
            break;
        case T_DICT:
            printf("\t%s\"%s\" : ", tabs, key.str);
            ro_dict_print_indent(it.dictv, indent + 1, 1);
            break;
        }

        if (i < size - 1)
        {
            printf(",\n");
        }
    }
    printf("\n%s}", tabs);
    free(tabs);
}

void ro_dict_print(ro_dict_t *d)
{
    if (d)
    {
#ifndef VALGRING_DISABLE_PRINT
        ro_dict_print_indent(d, 1, 0);
#endif
    }
}

/*******************************************************************************
**                                 DESTRUCTION                                **
*******************************************************************************/
void destroy_ro_array(ro_array_t *a)
{
    if (!a)
    {
        return;
    }

    ro_value_t *values = a->values;
    unsigned size = a->size;
    for (unsigned i = 0; i < size; ++i)
    {
        ro_value_t val = values[i];
        switch (val.type)
        {
        case T_STR:
            destroy_string(val.strv);
            break;
        case T_ARR:
            destroy_ro_array(val.arrayv);
            break;
        case T_DICT:
            destroy_ro_dict(val.dictv);
            break;
        }
    }
    free(values);
    free(a);
}

void destroy_ro_dict(ro_dict_t *d)
{
    if (!d)
    {
        return;
    }

    ro_item_t *items = d->items;
    unsigned size = d->size;
    for (unsigned i = 0; i < size; ++i)
    {
        ro_item_t it = items[i];
        destroy_string(it.key);
        switch (it.type)
        {
        case T_STR:
            destroy_string(it.strv);
            break;
        case T_ARR:
            destroy_ro_array(it.arrayv);
            break;
        case T_DICT:
            destroy_ro_dict(it.dictv);
            break;
        }
    }
    free(items);
    free(d);
}

void destroy_ro_json(ro_json_t *j)
{
    if (!j)
    {
        return;
    }

    if (j->is_array)
    {
        destroy_ro_array(j->array);
    }
    else
    {
        destroy_ro_dict(j->dict);
    }

    free(j);
}
