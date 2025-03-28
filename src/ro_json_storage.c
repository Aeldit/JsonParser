#include "ro_json_storage.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
ro_array_t init_ro_array_with(size_t size, ...)
{
    if (!size)
    {
        return RO_ARRAY(0);
    }

    ro_value_t *values = calloc(size, sizeof(ro_value_t));
    if (!values)
    {
        return RO_ARRAY(0);
    }

    va_list args;

    va_start(args, size);
    for (size_t i = 0; i < size; ++i)
    {
        values[i] = va_arg(args, ro_value_t);
    }

    va_end(args);
    return (ro_array_t){ .size = size, .values = values };
}

ro_dict_t init_ro_dict_with(size_t size, ...)
{
    if (!size)
    {
        return RO_DICT(0);
    }

    ro_item_t *items = calloc(size, sizeof(ro_item_t));
    if (!items)
    {
        return RO_DICT(0);
    }

    va_list args;

    va_start(args, size);
    for (size_t i = 0; i < size; ++i)
    {
        items[i] = va_arg(args, ro_item_t);
    }

    va_end(args);
    return (ro_dict_t){ .size = size, .items = items };
}

/*******************************************************************************
**                                    GETS                                    **
*******************************************************************************/
ro_value_t ro_array_get(ro_array_t a, size_t index)
{
    if (index >= a.size)
    {
        return ERROR_RO_VALUE;
    }

    return index < a.size ? a.values[index] : ERROR_RO_VALUE;
}

ro_item_t ro_dict_get(ro_dict_t d, string_t key)
{
    if (!key.str)
    {
        return ERROR_RO_ITEM;
    }

    ro_item_t *items = d.items;
    size_t size      = d.size;
    for (size_t i = 0; i < size; ++i)
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
void ro_dict_print_indent(ro_dict_t d, u16 indent, bool fromDict);

void ro_array_print_indent(ro_array_t a, u16 indent, bool fromDict)
{
    // Obtains the number of tab characters that will be printed
    char *tabs = calloc(indent, sizeof(char));
    if (!tabs)
    {
        return;
    }
    for (u16 i = 0; i < indent - 1; ++i)
    {
        tabs[i] = '\t';
    }
    tabs[indent - 1] = '\0';

    size_t size = a.size;
    // Empty array
    if (!size)
    {
        printf("%s[]", fromDict ? "" : tabs);
        free(tabs);
        return;
    }

    printf("%s[\n", fromDict ? "" : tabs);

    ro_value_t *values = a.values;
    for (size_t i = 0; i < size; ++i)
    {
        ro_value_t v = values[i];
        switch (v.type)
        {
        case T_ERROR:
            continue;

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
            printf(
                "\t%s%lde%ld", tabs, v.exp_longv.number, v.exp_longv.exponent
            );
            break;
        case T_EXP_DOUBLE:
            printf(
                "\t%s%fe%ld", tabs, v.exp_doublev.number, v.exp_doublev.exponent
            );
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

void ro_array_print(ro_array_t a)
{
#ifndef VALGRING_DISABLE_PRINT
    ro_array_print_indent(a, 1, false);
#endif
}

void ro_dict_print_indent(ro_dict_t d, u16 indent, bool fromDict)
{
    // Obtains the number of tab characters that will be printed
    char *tabs = calloc(indent, sizeof(char));
    if (!tabs)
    {
        return;
    }
    for (u16 i = 0; i < indent - 1; ++i)
    {
        tabs[i] = '\t';
    }
    tabs[indent - 1] = '\0';

    size_t size = d.size;
    if (size == 0)
    {
        printf("%s{}", fromDict ? "" : tabs);
        free(tabs);
        return;
    }

    printf("%s{\n", fromDict ? "" : tabs);

    ro_item_t *items = d.items;
    for (size_t i = 0; i < size; ++i)
    {
        ro_item_t it = items[i];
        char *key    = it.key.str;
        switch (it.type)
        {
        case T_ERROR:
            continue;

        case T_STR:
            printf(
                "\t%s\"%s\": \"%s\"", tabs, key, it.strv.str ? it.strv.str : ""
            );
            break;
        case T_LONG:
            printf("\t%s\"%s\": %ld", tabs, key, it.longv);
            break;
        case T_DOUBLE:
            printf("\t%s\"%s\": %f", tabs, key, it.doublev);
            break;
        case T_EXP_LONG:
            printf(
                "\t%s\"%s\": %lde%ld", tabs, key, it.exp_longv.number,
                it.exp_longv.exponent
            );
            break;
        case T_EXP_DOUBLE:
            printf(
                "\t%s\"%s\": %fe%ld", tabs, key, it.exp_doublev.number,
                it.exp_doublev.exponent
            );
            break;
        case T_BOOL:
            printf("\t%s\"%s\": %s", tabs, key, it.boolv ? "true" : "false");
            break;
        case T_NULL:
            printf("\t%s\"%s\": null", tabs, key);
            break;
        case T_ARR:
            printf("\t%s\"%s\": ", tabs, key);
            ro_array_print_indent(it.arrayv, indent + 1, 1);
            break;
        case T_DICT:
            printf("\t%s\"%s\": ", tabs, key);
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

void ro_dict_print(ro_dict_t d)
{
#ifndef VALGRING_DISABLE_PRINT
    ro_dict_print_indent(d, 1, false);
#endif
}

/*******************************************************************************
**                                 DESTRUCTION                                **
*******************************************************************************/
void destroy_ro_array(ro_array_t a)
{
    ro_value_t *values = a.values;
    size_t size        = a.size;
    for (size_t i = 0; i < size; ++i)
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
}

void destroy_ro_dict(ro_dict_t d)
{
    ro_item_t *items = d.items;
    size_t size      = d.size;
    for (size_t i = 0; i < size; ++i)
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
}

void destroy_ro_json(ro_json_t j)
{
    if (j.is_array)
    {
        destroy_ro_array(j.array);
    }
    else
    {
        destroy_ro_dict(j.dict);
    }
}
