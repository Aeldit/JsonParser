#include "parser.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdlib.h>

#include "utils.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
/*
** \brief Adds the key:pair value to the json dict
** \param type_ctrl_st  The control typedef struct of the correct type
** \param list The list in which to add the value
** \param append_type The append function for the current type
*/
#define ADD(type_ctrl_st, list, append_type)                                   \
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
    p->type = TYPE_STR;                                                        \
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
    ADD(str_control_st, strings, append_str)
}

char add_num(json_dict_st *jd, const char *key, long value)
{
    ADD(num_control_st, numbers, append_num)
}

char add_bool(json_dict_st *jd, const char *key,
              char value){ ADD(bool_control_st, booleans, append_bool) }

size_t get_nb_keys(json_dict_st *jd)
{
    return jd == NULL ? 0 : jd->keys == NULL ? 0 : jd->keys->nb_keys;
}

#include <stdio.h>
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
    destroy_bool_control(jd->booleans);
    //  TODO: Destroy the other types
    free(jd);
}
