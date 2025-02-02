#include "rw_json_equality.h"

#include "base_json_storage.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
char rw_arrays_equal(rw_array_t *a, rw_array_t *b)
{
    if (!a || !b)
    {
        return 0;
    }

    unsigned a_size = a->size;
    unsigned b_size = b->size;
    if (a_size != b_size)
    {
        return 0;
    }

    unsigned i = 0;
    value_link_t *link = a->head;
    while (link)
    {
        rw_value_t *values = link->values;
        for (unsigned a = 0; a < ARRAY_LEN; ++a, ++i)
        {
            rw_value_t a_val = values[a];
            if (a_val.type == T_ERROR)
            {
                continue;
            }
            rw_value_t b_val = rw_array_get(b, i);
            if (a_val.type != b_val.type)
            {
                return 0;
            }

            char is_equal = 0;
            switch (a_val.type)
            {
            case T_STR:
                is_equal = strings_equals(a_val.strv, b_val.strv);
                break;
            case T_LONG:
                is_equal = a_val.longv == b_val.longv;
                break;
            case T_DOUBLE:
                is_equal = a_val.doublev == b_val.doublev;
                break;
            case T_EXP_LONG:
                is_equal = exp_long_equals(a_val.exp_longv, b_val.exp_longv);
                break;
            case T_EXP_DOUBLE:
                is_equal =
                    exp_double_equals(a_val.exp_doublev, b_val.exp_doublev);
                break;
            case T_BOOL:
                is_equal = a_val.boolv == b_val.boolv;
                break;
            case T_NULL:
                is_equal = 1;
                break;
            case T_ARR:
                is_equal = rw_arrays_equal(a_val.arrayv, b_val.arrayv);
                break;
            case T_DICT:
                is_equal = rw_dicts_equal(a_val.dictv, b_val.dictv);
                break;
            }

            if (!is_equal)
            {
                return 0;
            }
        }
        link = link->next;
    }
    return 1;
}

char rw_dicts_equal(rw_dict_t *a, rw_dict_t *b)
{
    if (!a || !b)
    {
        return 0;
    }

    unsigned a_size = a->size;
    unsigned b_size = b->size;
    if (a_size != b_size)
    {
        return 0;
    }

    unsigned i = 0;
    item_link_t *link = a->head;
    while (link)
    {
        rw_item_t *items = link->items;
        for (unsigned a = 0; a < ARRAY_LEN; ++a, ++i)
        {
            rw_item_t a_it = items[a];
            if (a_it.type == T_ERROR)
            {
                continue;
            }
            rw_item_t b_it = rw_dict_get(b, a_it.key);

            // If the second dict doesn't contain the current key
            // OR
            // If the second dict contains the key but the associated
            // element is not of the same type
            if (a_it.type != b_it.type)
            {
                return 0;
            }

            char is_equal = 0;
            switch (a_it.type)
            {
            case T_STR:
                is_equal = strings_equals(a_it.strv, b_it.strv);
                break;
            case T_LONG:
                is_equal = a_it.longv == b_it.longv;
                break;
            case T_DOUBLE:
                is_equal = a_it.doublev == b_it.doublev;
                break;
            case T_EXP_LONG:
                is_equal = exp_long_equals(a_it.exp_longv, b_it.exp_longv);
                break;
            case T_EXP_DOUBLE:
                is_equal =
                    exp_double_equals(a_it.exp_doublev, b_it.exp_doublev);
                break;
            case T_BOOL:
                is_equal = a_it.boolv == b_it.boolv;
                break;
            case T_NULL:
                is_equal = 1;
                break;
            case T_ARR:
                is_equal = rw_arrays_equal(a_it.arrayv, b_it.arrayv);
                break;
            case T_DICT:
                is_equal = rw_dicts_equal(a_it.dictv, b_it.dictv);
                break;
            }

            if (!is_equal)
            {
                return 0;
            }
        }
        link = link->next;
    }
    return 1;
}

char rw_json_equal(rw_json_t *a, rw_json_t *b)
{
    if (!a || !b || a->is_array != b->is_array)
    {
        return 0;
    }
    return a->is_array ? rw_arrays_equal(a->array, b->array)
                       : rw_dicts_equal(a->dict, b->dict);
}
