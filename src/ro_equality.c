#include "ro_equality.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
inline char exp_long_equals(exponent_long_t a, exponent_long_t b)
{
    return a.number == b.number && a.exponent == b.exponent;
}

inline char exp_double_equals(exponent_double_t a, exponent_double_t b)
{
    return a.number == b.number && a.exponent == b.exponent;
}

char ro_arrays_equal(ro_array_t *a, ro_array_t *b)
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

    ro_value_t *a_values = a->values;
    ro_value_t *b_values = b->values;
    for (unsigned i = 0; i < a_size; ++i)
    {
        ro_value_t a_val = a_values[i];
        ro_value_t b_val = b_values[i];
        if (a_val.type != b_val.type)
        {
            return 0;
        }

        char is_equal = 0;
        switch (a_val.type)
        {
        case T_ERROR:
            continue;
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
            is_equal = exp_double_equals(a_val.exp_doublev, b_val.exp_doublev);
            break;
        case T_BOOL:
            is_equal = a_val.boolv == b_val.boolv;
            break;
        case T_NULL:
            is_equal = 1;
            break;
        case T_ARR:
            is_equal = ro_arrays_equal(a_val.arrayv, b_val.arrayv);
            break;
        case T_DICT:
            is_equal = ro_dicts_equal(a_val.dictv, b_val.dictv);
            break;
        }

        if (!is_equal)
        {
            return 0;
        }
    }
    return 1;
}

char ro_dicts_equal(ro_dict_t *a, ro_dict_t *b)
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

    ro_item_t *a_items = a->items;
    for (unsigned i = 0; i < a_size; ++i)
    {
        ro_item_t a_it = a_items[i];
        ro_item_t b_it = ro_dict_get(b, a_it.key);

        // If the second dict doesn't contain the current key
        if (b_it.type == T_ERROR)
        {
            return 0;
        }

        // If the second dict contains the key but the associated element is not
        // of the same type
        if (a_it.type != b_it.type)
        {
            return 0;
        }

        char is_equal = 0;
        switch (a_it.type)
        {
        case T_ERROR:
            continue;
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
            is_equal = exp_double_equals(a_it.exp_doublev, b_it.exp_doublev);
            break;
        case T_BOOL:
            is_equal = a_it.boolv == b_it.boolv;
            break;
        case T_NULL:
            is_equal = 1;
            break;
        case T_ARR:
            is_equal = ro_arrays_equal(a_it.arrayv, b_it.arrayv);
            break;
        case T_DICT:
            is_equal = ro_dicts_equal(a_it.dictv, b_it.dictv);
            break;
        }

        if (!is_equal)
        {
            return 0;
        }
    }
    return 1;
}
