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
