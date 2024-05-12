#include "linked_lists.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "../json.h"
#include "generic_lists.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
/**
** \brief Destroys the given linked list
** \param link_type The typedef struct
** \param st The name of the variable of the type 'st_name'
** \param elt The element to add
** \param list the name of the list in which to add 'elt'
** \param nb_type The name of the field containing the number of elements
*/
#define APPEND(link_type, list, nb_type)                                       \
    if (ctrl == NULL)                                                          \
    {                                                                          \
        return NULL;                                                           \
    }                                                                          \
    /* Case where there is no element */                                       \
    if (ctrl->head == NULL)                                                    \
    {                                                                          \
        ctrl->head = calloc(1, sizeof(struct link_type));                      \
        if (ctrl->head == NULL)                                                \
        {                                                                      \
            return NULL;                                                       \
        }                                                                      \
        ctrl->head->list[ctrl->idx] = value;                                   \
    }                                                                          \
    /* Case where the current array is full */                                 \
    else if (ctrl->idx == ARRAY_LEN)                                           \
    {                                                                          \
        /* We allocate the new element, add it at the end of the linked list   \
           of its type and adds to it the given key */                         \
        struct link_type *nkl = calloc(1, sizeof(struct link_type));           \
        if (nkl == NULL)                                                       \
        {                                                                      \
            return NULL;                                                       \
        }                                                                      \
        struct link_type *tmp = ctrl->head;                                    \
        while (tmp->next != NULL)                                              \
        {                                                                      \
            tmp = tmp->next;                                                   \
        }                                                                      \
        tmp->next = nkl;                                                       \
        ctrl->idx = 0;                                                         \
        nkl->list[ctrl->idx] = value;                                          \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        /* Adds the given key to the last array */                             \
        struct link_type *tmp = ctrl->head;                                    \
        while (tmp->next != NULL)                                              \
        {                                                                      \
            tmp = tmp->next;                                                   \
        }                                                                      \
        tmp->list[ctrl->idx] = value;                                          \
    }                                                                          \
    ++ctrl->nb_type;                                                           \
    struct link_type *tmp = ctrl->head;                                        \
    while (tmp->next != NULL)                                                  \
    {                                                                          \
        tmp = tmp->next;                                                       \
    }

/*
** \brief Destroys the given linked list
** \param link_type The typedef struct
*/
#define DESTROY(link_type)                                                     \
    if (ctrl == NULL)                                                          \
    {                                                                          \
        return;                                                                \
    }                                                                          \
    struct link_type *tmp = ctrl->head;                                        \
    while (tmp != NULL)                                                        \
    {                                                                          \
        struct link_type *to_del = tmp;                                        \
        tmp = tmp->next;                                                       \
        free(to_del);                                                          \
    }                                                                          \
    free(ctrl);

/*******************************************************************************
**                              LOCAL FUNCTIONS                               **
*******************************************************************************/
void print_list(struct generic_list *l, char indent, char from_list)
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
                print_list(l->elts[i].value, indent + 1, 1);
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

void print_json_rec(pair_control_st *ctrl, char indent)
{
    if (ctrl == NULL || ctrl->head == NULL || ctrl->nb_pairs == 0)
    {
        return;
    }

    char *tabs = calloc(indent + 1, sizeof(char));
    if (tabs == NULL)
    {
        return;
    }
    for (int i = 0; i < indent - 1; ++i)
    {
        tabs[i] = '\t';
    }

    struct pair_array_link *array = ctrl->head;
    printf("{\n");
    // Iterates over the arrays
    while (array != NULL)
    {
        // Iterates over an array
        for (size_t i = 0; i < (array->next == NULL ? ctrl->idx : ARRAY_LEN);
             ++i)
        {
            if (array->pairs[i] != NULL)
            {
                const char *key = array->pairs[i]->key;
                long num = 0;
                char boolean = 0;
                generic_list_st *l = NULL;
                json_dict_st *j = NULL;
                switch (array->pairs[i]->type)
                {
                case TYPE_STR:
                    printf("%s\t\"%s\": \"%s\"", tabs, key,
                           (const char *)array->pairs[i]->value);
                    break;
                case TYPE_NUM:
                    num = *(long *)array->pairs[i]->value;
                    printf("%s\t\"%s\": %ld", tabs, key, num);
                    break;
                case TYPE_OBJ:
                    j = array->pairs[i]->value;
                    printf("%s\t\"%s\": ", tabs, key);
                    if (j == NULL)
                    {
                        printf("{}");
                    }
                    else
                    {
                        print_json_rec(j->pairs, indent + 1);
                    }
                    break;
                case TYPE_ARR:
                    l = array->pairs[i]->value;
                    printf("%s\t\"%s\": ", tabs, key);
                    print_list(l, indent + 1, 0);
                    break;
                case TYPE_BOOL:
                    boolean = *(char *)array->pairs[i]->value;
                    printf("%s\t\"%s\": %s", tabs, key,
                           boolean ? "true" : "false");
                    break;
                case TYPE_NULL:
                    printf("%s\t\"%s\": null", tabs, key);
                    break;
                default:
                    break;
                }

                // If not at the last element of the last array, we print a ','
                if ((array->next == NULL && i != ctrl->idx - 1)
                    || array->next != NULL)
                {
                    printf(",\n");
                }
            }
        }
        array = array->next;
    }
    printf("\n%s}", tabs);
    if (indent == 1)
    {
        printf("\n");
    }
    free(tabs);
}

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/***************************************
**               PAIR                 **
***************************************/
struct pair *append_pair(pair_control_st *ctrl, struct pair *value)
{
    if (value == NULL)
    {
        return NULL;
    }
    APPEND(pair_array_link, pairs, nb_pairs)
    return tmp->pairs[ctrl->idx++];
}

void print_json(pair_control_st *ctrl)
{
    print_json_rec(ctrl, 1);
}

void destroy_pair_control(pair_control_st *ctrl)
{
    if (ctrl == NULL)
    {
        return;
    }

    struct pair_array_link *tmp = ctrl->head;
    while (tmp != NULL)
    {
        struct pair_array_link *to_del = tmp;
        tmp = tmp->next;
        for (size_t i = 0; i < ARRAY_LEN; ++i)
        {
            free(to_del->pairs[i]);
        }
        free(to_del);
    }
    free(ctrl);
}

/***************************************
**                KEY                 **
***************************************/
const char *append_key(key_control_st *ctrl, const char *value)
{
    if (value == NULL)
    {
        return NULL;
    }
    APPEND(key_array_link, keys, nb_keys)
    return tmp->keys[ctrl->idx++];
}

void destroy_key_control(key_control_st *ctrl)
{
    DESTROY(key_array_link)
}

/***************************************
**                STR                 **
***************************************/
char *append_str(str_control_st *ctrl, char *value)
{
    if (value == NULL)
    {
        return NULL;
    }
    APPEND(str_array_link, strings, nb_str)
    return tmp->strings[ctrl->idx++];
}

void destroy_str_control(str_control_st *ctrl)
{
    DESTROY(str_array_link)
}

/***************************************
**                NUM                 **
***************************************/
long *append_num(num_control_st *ctrl, long value)
{
    APPEND(num_array_link, numbers, nb_num)
    return &tmp->numbers[ctrl->idx++];
}

void destroy_num_control(num_control_st *ctrl){ DESTROY(num_array_link) }

/***************************************
**             JSON DICT              **
***************************************/
json_dict_st *append_json_dict(json_dict_control_st *ctrl, json_dict_st *value)
{
    APPEND(json_dict_array_link, json_dicts, nb_json_dicts)
    return tmp->json_dicts[ctrl->idx++];
}

void destroy_json_dict_control(json_dict_control_st *ctrl)
{
    if (ctrl == NULL)
    {
        return;
    }

    struct json_dict_array_link *tmp = ctrl->head;
    while (tmp != NULL)
    {
        struct json_dict_array_link *to_del = tmp;
        tmp = tmp->next;
        for (size_t i = 0; i < ARRAY_LEN; ++i)
        {
            destroy_dict(to_del->json_dicts[i]);
        }
        free(to_del);
    }
    free(ctrl);
}

/***************************************
**                LIST                **
***************************************/
generic_list_st *append_list(list_control_st *ctrl, generic_list_st *value)
{
    APPEND(list_array_link, lists, nb_arr)
    return tmp->lists[ctrl->idx++];
}

void destroy_list_control(list_control_st *ctrl)
{
    if (ctrl == NULL)
    {
        return;
    }

    struct list_array_link *tmp = ctrl->head;
    while (tmp != NULL)
    {
        struct list_array_link *to_del = tmp;
        tmp = tmp->next;
        for (size_t i = 0; i < ARRAY_LEN; ++i)
        {
            if (to_del->lists[i] != NULL)
            {
                free(to_del->lists[i]->elts);
            }
            free(to_del->lists[i]);
        }
        free(to_del);
    }
    free(ctrl);
}

/***************************************
**               BOOL                 **
***************************************/
char *append_bool(bool_control_st *ctrl, char value)
{
    APPEND(bool_array_link, booleans, nb_bool)
    return &tmp->booleans[ctrl->idx++];
}

void destroy_bool_control(bool_control_st *ctrl)
{
    DESTROY(bool_array_link)
}
