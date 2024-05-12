#include "json.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdlib.h>

#include "lists/linked_lists.h"
#include "utils.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
/*
** \brief Adds the key:pair value to the json dict
** \param type_ctrl_st  The control typedef struct of the correct type
** \param list The list in which to add the value
** \param append_type The append function for the current type
** \param value_type The type of the value
*/
#define ADD(type_ctrl_st, list, append_type, value_type)                       \
    if (jd == NULL || key == NULL)                                             \
    {                                                                          \
        return FAILURE;                                                        \
    }                                                                          \
    if (jd->pairs == NULL)                                                     \
    {                                                                          \
        jd->pairs = calloc(1, sizeof(pair_control_st));                        \
    }                                                                          \
    if (jd->keys == NULL)                                                      \
    {                                                                          \
        jd->keys = calloc(1, sizeof(key_control_st));                          \
    }                                                                          \
    if (jd->list == NULL)                                                      \
    {                                                                          \
        jd->list = calloc(1, sizeof(type_ctrl_st));                            \
    }                                                                          \
    if (jd->pairs == NULL || jd->keys == NULL || jd->list == NULL)             \
    {                                                                          \
        destroy_dict(jd);                                                      \
        return FAILURE;                                                        \
    }                                                                          \
    struct pair *p = calloc(1, sizeof(struct pair));                           \
    if (p == NULL)                                                             \
    {                                                                          \
        return FAILURE;                                                        \
    }                                                                          \
    p->type = (value_type);                                                    \
    p->key = append_key(jd->keys, key);                                        \
    p->value = append_type(jd->list, value);                                   \
    append_pair(jd->pairs, p);                                                 \
    return SUCCESS;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
json_dict_st *init_dict(void)
{
    json_dict_st *jd = calloc(1, sizeof(json_dict_st));
    return jd == NULL ? NULL : jd;
}

// TODO: Prevent duplicated keys from being inserted

char add_str(json_dict_st *jd, const char *key, char *value)
{
    if (value == NULL)
    {
        return FAILURE;
    }
    ADD(str_control_st, strings, append_str, TYPE_STR)
}

char add_num(json_dict_st *jd, const char *key, long value)
{
    ADD(num_control_st, numbers, append_num, TYPE_NUM)
}

char add_json_dict(json_dict_st *jd, const char *key, json_dict_st *dict)
{
    if (jd == NULL || key == NULL || dict == NULL)
    {
        return FAILURE;
    }

    if (jd->pairs == NULL)
    {
        jd->pairs = calloc(1, sizeof(pair_control_st));
    }
    if (jd->keys == NULL)
    {
        jd->keys = calloc(1, sizeof(key_control_st));
    }
    if (jd->json_dicts == NULL)
    {
        jd->json_dicts = calloc(1, sizeof(json_dict_control_st));
    }
    if (jd->pairs == NULL || jd->keys == NULL || jd->json_dicts == NULL)
    {
        destroy_dict(jd);
        return FAILURE;
    }

    struct pair *p = calloc(1, sizeof(struct pair));
    if (p == NULL)
    {
        return FAILURE;
    }
    p->type = TYPE_OBJ;
    p->key = append_key(jd->keys, key);
    p->value = append_json_dict(jd->json_dicts, dict);
    append_pair(jd->pairs, p);
    return SUCCESS;
}

char add_list(json_dict_st *jd, const char *key, struct generic_list *list)
{
    if (jd == NULL || key == NULL || list == NULL)
    {
        return FAILURE;
    }

    if (jd->pairs == NULL)
    {
        jd->pairs = calloc(1, sizeof(pair_control_st));
    }
    if (jd->keys == NULL)
    {
        jd->keys = calloc(1, sizeof(key_control_st));
    }
    if (jd->lists == NULL)
    {
        jd->lists = calloc(1, sizeof(list_control_st));
    }
    if (jd->pairs == NULL || jd->keys == NULL || jd->lists == NULL)
    {
        destroy_dict(jd);
        return FAILURE;
    }

    struct pair *p = calloc(1, sizeof(struct pair));
    if (p == NULL)
    {
        return FAILURE;
    }
    p->type = TYPE_ARR;
    p->key = append_key(jd->keys, key);
    p->value = append_list(jd->lists, list);
    append_pair(jd->pairs, p);
    return SUCCESS;
}

char add_bool(json_dict_st *jd, const char *key, char value)
{
    ADD(bool_control_st, booleans, append_bool, TYPE_BOOL)
}

char add_null(json_dict_st *jd, const char *key)
{
    if (jd == NULL || key == NULL)
    {
        return FAILURE;
    }
    struct pair *p = calloc(1, sizeof(struct pair));
    if (p == NULL)
    {
        return FAILURE;
    }
    p->type = TYPE_NULL;
    p->key = key;
    p->value = NULL;
    append_pair(jd->pairs, p);
    return SUCCESS;
}

size_t get_nb_keys(json_dict_st *jd)
{
    return jd == NULL ? 0 : jd->keys == NULL ? 0 : jd->keys->nb_keys;
}

void destroy_dict(json_dict_st *jd)
{
    if (jd == NULL)
    {
        return;
    }

    destroy_pair_control(jd->pairs);
    destroy_key_control(jd->keys);

    destroy_str_control(jd->strings);
    destroy_num_control(jd->numbers);
    destroy_json_dict_control(jd->json_dicts);
    destroy_list_control(jd->lists);
    destroy_bool_control(jd->booleans);
    free(jd);
}
