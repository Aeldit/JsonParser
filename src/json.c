#include "json.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdlib.h>

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

char add_num(json_dict_st *jd, char *key, long value)
{
    ADD(num_control_st, numbers, append_num, TYPE_NUM)
}

char add_json_dict(json_dict_st *jd, char *key, json_dict_st *value)
{
    if (value == NULL)
    {
        return FAILURE;
    }
    ADD(json_dict_control_st, json_dicts, append_json_dict, TYPE_OBJ)
}

char add_array(json_dict_st *jd, char *key, json_array_st *value)
{
    if (value == NULL)
    {
        return FAILURE;
    }
    ADD(list_control_st, lists, append_array, TYPE_ARR)
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
        (struct array_elt){ .value = append_str(jd, value), .type = TYPE_STR });
    return SUCCESS;
}

char add_num_to_array(json_dict_st *jd, json_array_st *ja, long value)
{
    if (jd == NULL || ja == NULL)
    {
        return FAILURE;
    }

    if (jd->numbers == NULL)
    {
        jd->numbers = calloc(1, sizeof(num_control_st));
        if (jd->numbers == NULL)
        {
            destroy_dict(jd);
            return FAILURE;
        }
    }

    array_append(
        ja,
        (struct array_elt){ .value = append_num(jd, value), .type = TYPE_NUM });
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
                 (struct array_elt){ .value = append_json_dict(jd, value),
                                     .type = TYPE_OBJ });
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

    array_append(ja,
                 (struct array_elt){ .value = append_array(jd, value),
                                     .type = TYPE_ARR });
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

    array_append(ja,
                 (struct array_elt){ .value = append_bool(jd, value),
                                     .type = TYPE_BOOL });
    return SUCCESS;
}

char add_null_to_array(json_dict_st *jd, json_array_st *ja)
{
    if (jd == NULL || ja == NULL)
    {
        return FAILURE;
    }

    array_append(ja, (struct array_elt){ .value = NULL, .type = TYPE_NULL });
    return SUCCESS;
}

size_t get_nb_items(json_dict_st *jd)
{
    return jd == NULL ? 0 : jd->nb_items;
}
