#include "json.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdlib.h>

#include "json_api.h"
#include "lists/linked_lists.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
/*
** \def Adds the key:item value to the json dict
** \param type_ctrl_st  The control typedef struct of the correct type
** \param list The list in which to add the value
** \param append_type The append function for the current type
** \param value_type The type of the value
*/
#define ADD(type_ctrl_st, list, append_type, value_type)                       \
    if (jd == NULL || key == NULL || key_exists(jd, key))                      \
    {                                                                          \
        return FAILURE;                                                        \
    }                                                                          \
    if (jd->items == NULL)                                                     \
    {                                                                          \
        jd->items = calloc(1, sizeof(item_control_st));                        \
    }                                                                          \
    if (jd->keys == NULL)                                                      \
    {                                                                          \
        jd->keys = calloc(1, sizeof(key_control_st));                          \
    }                                                                          \
    if (jd->list == NULL)                                                      \
    {                                                                          \
        jd->list = calloc(1, sizeof(type_ctrl_st));                            \
    }                                                                          \
    if (jd->items == NULL || jd->keys == NULL || jd->list == NULL)             \
    {                                                                          \
        destroy_dict(jd);                                                      \
        return FAILURE;                                                        \
    }                                                                          \
    struct item *p = calloc(1, sizeof(struct item));                           \
    if (p == NULL)                                                             \
    {                                                                          \
        return FAILURE;                                                        \
    }                                                                          \
    p->type = (value_type);                                                    \
    p->key = append_key(jd, key);                                              \
    p->value = append_type(jd, value);                                         \
    append_item(jd, p);                                                        \
    return SUCCESS;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
json_dict_st *init_dict(void)
{
    json_dict_st *jd = calloc(1, sizeof(json_dict_st));
    return jd == NULL ? NULL : jd;
}

char add_str(json_dict_st *jd, char *key, char *value)
{
    if (value == NULL)
    {
        return FAILURE;
    }
    ADD(str_control_st, strings, append_str, TYPE_STR)
}

char add_int(json_dict_st *jd, char *key, int64_t value)
{
    ADD(int64_t_control_st, integers, append_int, TYPE_INT)
}

char add_double(json_dict_st *jd, char *key, double value)
{
    ADD(double_control_st, doubles, append_double, TYPE_DOUBLE)
}

char add_bool(json_dict_st *jd, char *key, char value)
{
    ADD(bool_control_st, booleans, append_bool, TYPE_BOOL)
}

char add_null(json_dict_st *jd, char *key)
{
    if (jd == NULL || key == NULL || key_exists(jd, key))
    {
        return FAILURE;
    }

    struct item *p = calloc(1, sizeof(struct item));
    if (p == NULL)
    {
        return FAILURE;
    }

    p->type = TYPE_NULL;
    p->key = key;
    p->value = NULL;
    append_item(jd, p);
    return SUCCESS;
}

char add_array(json_dict_st *jd, char *key, json_array_st *value)
{
    if (value == NULL)
    {
        return FAILURE;
    }
    ADD(list_control_st, lists, append_array, TYPE_ARR)
}

char add_json_dict(json_dict_st *jd, char *key, json_dict_st *value)
{
    if (value == NULL)
    {
        return FAILURE;
    }
    ADD(json_dict_control_st, json_dicts, append_json_dict, TYPE_DICT)
}

/***************************************
**                ARRAY               **
***************************************/
char add_str_to_array(json_dict_st *jd, json_array_st *ja, char *value)
{
    if (jd == NULL || ja == NULL || value == NULL)
    {
        return FAILURE;
    }

    if (jd->strings == NULL)
    {
        jd->strings = calloc(1, sizeof(str_control_st));
        if (jd->strings == NULL)
        {
            destroy_dict(jd);
            return FAILURE;
        }
    }

    array_append(
        ja,
        (typed_value_st){ .value = append_str(jd, value), .type = TYPE_STR });
    return SUCCESS;
}

char add_int_to_array(json_dict_st *jd, json_array_st *ja, int64_t value)
{
    if (jd == NULL || ja == NULL)
    {
        return FAILURE;
    }

    if (jd->integers == NULL)
    {
        jd->integers = calloc(1, sizeof(int64_t_control_st));
        if (jd->integers == NULL)
        {
            destroy_dict(jd);
            return FAILURE;
        }
    }

    array_append(
        ja,
        (typed_value_st){ .value = append_int(jd, value), .type = TYPE_INT });
    return SUCCESS;
}

char add_double_to_array(json_dict_st *jd, json_array_st *ja, double value)
{
    if (jd == NULL || ja == NULL)
    {
        return FAILURE;
    }

    if (jd->doubles == NULL)
    {
        jd->doubles = calloc(1, sizeof(double_control_st));
        if (jd->doubles == NULL)
        {
            destroy_dict(jd);
            return FAILURE;
        }
    }

    array_append(ja,
                 (typed_value_st){ .value = append_double(jd, value),
                                   .type = TYPE_DOUBLE });
    return SUCCESS;
}

char add_bool_to_array(json_dict_st *jd, json_array_st *ja, char value)
{
    if (jd == NULL || ja == NULL)
    {
        return FAILURE;
    }

    if (jd->booleans == NULL)
    {
        jd->booleans = calloc(1, sizeof(bool_control_st));
        if (jd->booleans == NULL)
        {
            destroy_dict(jd);
            return FAILURE;
        }
    }

    array_append(
        ja,
        (typed_value_st){ .value = append_bool(jd, value), .type = TYPE_BOOL });
    return SUCCESS;
}

char add_null_to_array(json_dict_st *jd, json_array_st *ja)
{
    if (jd == NULL || ja == NULL)
    {
        return FAILURE;
    }

    array_append(ja, (typed_value_st){ .value = NULL, .type = TYPE_NULL });
    return SUCCESS;
}

char add_array_to_array(json_dict_st *jd, json_array_st *ja,
                        json_array_st *value)
{
    if (jd == NULL || ja == NULL || value == NULL)
    {
        return FAILURE;
    }

    if (jd->lists == NULL)
    {
        jd->lists = calloc(1, sizeof(list_control_st));
        if (jd->lists == NULL)
        {
            destroy_dict(jd);
            return FAILURE;
        }
    }

    array_append(
        ja,
        (typed_value_st){ .value = append_array(jd, value), .type = TYPE_ARR });
    return SUCCESS;
}

char add_json_dict_to_array(json_dict_st *jd, json_array_st *ja,
                            json_dict_st *value)
{
    if (jd == NULL || ja == NULL || value == NULL)
    {
        return FAILURE;
    }

    if (jd->json_dicts == NULL)
    {
        jd->json_dicts = calloc(1, sizeof(json_dict_control_st));
        if (jd->json_dicts == NULL)
        {
            destroy_dict(jd);
            return FAILURE;
        }
    }

    array_append(ja,
                 (typed_value_st){ .value = append_json_dict(jd, value),
                                   .type = TYPE_DICT });
    return SUCCESS;
}

size_t get_nb_items(json_dict_st *jd)
{
    return jd == NULL ? 0 : jd->nb_items;
}
