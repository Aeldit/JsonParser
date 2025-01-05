#ifndef BASE_JSON_WRITE_H
#define BASE_JSON_WRITE_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base_json_storage.h"

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
typedef struct str_link
{
    string_t s;
    char s_needs_free;
    char is_from_str;
    struct str_link *next;
} string_link_t;

typedef struct
{
    string_link_t *head;
    string_link_t *tail;
} string_linked_list_t;

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define HANDLE_VALUES_FOR_MODE(rx_value_t, get_rx_array_as_str,                \
                               get_rx_dict_as_str)                             \
    unsigned nb = 0;                                                           \
    for (unsigned i = 0; i < size; ++i)                                        \
    {                                                                          \
        rx_value_t v = values[i];                                              \
        if (v.type == T_ERROR)                                                 \
        {                                                                      \
            continue;                                                          \
        }                                                                      \
        string_t tmp_str;                                                      \
        switch (v.type)                                                        \
        {                                                                      \
        case T_STR:                                                            \
            tmp_str = v.strv;                                                  \
            nb += 2; /* Strings are encased by 2 double-quotes (\"\")*/        \
            break;                                                             \
        case T_LONG:                                                           \
            tmp_str = get_long_as_str(v.longv);                                \
            break;                                                             \
        case T_DOUBLE:                                                         \
            tmp_str = get_double_as_str(v.doublev);                            \
            break;                                                             \
        case T_EXP_LONG:                                                       \
            tmp_str = get_exp_long_as_str(v.exp_longv);                        \
            break;                                                             \
        case T_EXP_DOUBLE:                                                     \
            tmp_str = get_exp_double_as_str(v.exp_doublev);                    \
            break;                                                             \
        case T_BOOL:                                                           \
            tmp_str = get_bool_as_str(v.boolv);                                \
            break;                                                             \
        case T_NULL:                                                           \
            tmp_str = get_null_as_str();                                       \
            break;                                                             \
        case T_ARR:                                                            \
            tmp_str = get_rx_array_as_str(v.arrayv, indent + 1);               \
            break;                                                             \
        case T_DICT:                                                           \
            tmp_str = get_rx_dict_as_str(v.dictv, indent + 1);                 \
            break;                                                             \
        }                                                                      \
        add_link(ll, tmp_str, v.type != T_STR, v.type == T_STR);               \
        /* We add 1 for the comma if we are not at the last value */           \
        /* We add 1 for the line return */                                     \
        /* We add 'indent' for the tabs */                                     \
        nb += tmp_str.len + (i < total_size ? 1 : 0) + 1 + indent * 4;         \
    }

#define HANDLE_ITEMS_FOR_MODE(rx_item_t, get_rx_array_as_str,                  \
                              get_rx_dict_as_str)                              \
    unsigned nb = 0;                                                           \
    char is_key = 1;                                                           \
    for (unsigned i = 0; i < size; ++i)                                        \
    {                                                                          \
        rx_item_t it = items[i];                                               \
        if (it.type == T_ERROR)                                                \
        {                                                                      \
            continue;                                                          \
        }                                                                      \
        string_t tmp_str;                                                      \
        switch (it.type)                                                       \
        {                                                                      \
        case T_STR:                                                            \
            tmp_str = it.strv;                                                 \
            nb += 2; /* Strings are encased by 2 double-quotes (\"\") */       \
            break;                                                             \
        case T_LONG:                                                           \
            tmp_str = get_long_as_str(it.longv);                               \
            break;                                                             \
        case T_DOUBLE:                                                         \
            tmp_str = get_double_as_str(it.doublev);                           \
            break;                                                             \
        case T_EXP_LONG:                                                       \
            tmp_str = get_exp_long_as_str(it.exp_longv);                       \
            break;                                                             \
        case T_EXP_DOUBLE:                                                     \
            tmp_str = get_exp_double_as_str(it.exp_doublev);                   \
            break;                                                             \
        case T_BOOL:                                                           \
            tmp_str = get_bool_as_str(it.boolv);                               \
            break;                                                             \
        case T_NULL:                                                           \
            tmp_str = get_null_as_str();                                       \
            break;                                                             \
        case T_ARR:                                                            \
            tmp_str = get_rx_array_as_str(it.arrayv, indent + 1);              \
            break;                                                             \
        case T_DICT:                                                           \
            tmp_str = get_rx_dict_as_str(it.dictv, indent + 1);                \
            break;                                                             \
        }                                                                      \
        add_link(ll, it.key, 0, 1);                                            \
        /* + 4 because we add '": "' after the key */                          \
        nb += it.key.len + 4;                                                  \
        add_link(ll, tmp_str, it.type != T_STR, it.type == T_STR);             \
        /* We add 1 for the comma if we are not at the last value */           \
        /* We add 1 for the line return */                                     \
        /* We add 'indent' for the tabs */                                     \
        nb += tmp_str.len + (i < total_size ? 1 : 0) + 1 + indent * 4;         \
        is_key = !is_key;                                                      \
    }

#define GET_ARRAY_AS_STR(fill_rx_string_ll_with_values)                        \
    if (!a)                                                                    \
    {                                                                          \
        return NULL_STRING;                                                    \
    }                                                                          \
    if (!a->size)                                                              \
    {                                                                          \
        char *str = calloc(3, sizeof(char));                                   \
        if (!str)                                                              \
        {                                                                      \
            return NULL_STRING;                                                \
        }                                                                      \
        memcpy(str, "[]", 2);                                                  \
        return STRING_OF(str, 2);                                              \
    }                                                                          \
    string_linked_list_t *ll = calloc(1, sizeof(string_linked_list_t));        \
    if (!ll)                                                                   \
    {                                                                          \
        return NULL_STRING;                                                    \
    }                                                                          \
    /* '[' + '\n' + (indent - 1) * 4 * ' ' + ']' + '\n'*/                      \
    /* indents are 4 spaces */                                                 \
    /* indent == 1 -> if we are in the 'root' array, we add a '\n' at the */   \
    /* end */                                                                  \
    unsigned nb_chars = 2 + (indent - 1) * 4 + (indent == 1)                   \
        + fill_rx_string_ll_with_values(ll, a, indent);                        \
    unsigned nb_chars_indent = indent * 4;                                     \
    char *str = calloc(nb_chars + 1, sizeof(char));                            \
    if (!str)                                                                  \
    {                                                                          \
        destroy_linked_list(ll);                                               \
        return NULL_STRING;                                                    \
    }                                                                          \
    /* |-> Start building the string */                                        \
    str[0] = '[';                                                              \
    str[1] = '\n';                                                             \
    unsigned insert_idx = 2;                                                   \
    string_link_t *link = ll->head;                                            \
    while (link)                                                               \
    {                                                                          \
        /* Tabs */                                                             \
        memset(str + insert_idx, ' ', nb_chars_indent);                        \
        insert_idx += nb_chars_indent;                                         \
        if (link->is_from_str)                                                 \
        {                                                                      \
            str[insert_idx++] = '"';                                           \
        }                                                                      \
        /* Value as string */                                                  \
        memcpy(str + insert_idx, link->s.str, link->s.len);                    \
        insert_idx += link->s.len;                                             \
        if (link->is_from_str)                                                 \
        {                                                                      \
            str[insert_idx++] = '"';                                           \
        }                                                                      \
        /* Comma and line return */                                            \
        if (link->next)                                                        \
        {                                                                      \
            str[insert_idx++] = ',';                                           \
        }                                                                      \
        str[insert_idx++] = '\n';                                              \
        link = link->next;                                                     \
    }                                                                          \
    if (indent == 1)                                                           \
    {                                                                          \
        str[nb_chars - 2] = ']';                                               \
        str[nb_chars - 1] = '\n';                                              \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        /* Tabs before the last ']' */                                         \
        memset(str + insert_idx, ' ', nb_chars_indent - 4);                    \
        insert_idx += nb_chars_indent - 4;                                     \
        str[nb_chars - 1] = ']';                                               \
    }                                                                          \
    /* |-> End of string building */                                           \
    destroy_linked_list(ll);                                                   \
    return STRING_OF(str, nb_chars)

#define GET_DICT_AS_STR(fill_rx_string_ll_with_items)                          \
    if (!d)                                                                    \
    {                                                                          \
        return NULL_STRING;                                                    \
    }                                                                          \
    if (!d->size)                                                              \
    {                                                                          \
        char *str = calloc(3, sizeof(char));                                   \
        if (!str)                                                              \
        {                                                                      \
            return NULL_STRING;                                                \
        }                                                                      \
        memcpy(str, "{}", 2);                                                  \
        return STRING_OF(str, 2);                                              \
    }                                                                          \
    string_linked_list_t *ll = calloc(1, sizeof(string_linked_list_t));        \
    if (!ll)                                                                   \
    {                                                                          \
        return NULL_STRING;                                                    \
    }                                                                          \
    /* '{' + '\n' + (indent - 1) * '\t' + '}' + '\n'*/                         \
    /* indent == 1 -> if we are in the 'root' dict, we add a \n at the end */  \
    unsigned nb_chars = 2 + (indent - 1) * 4 + (indent == 1)                   \
        + fill_rx_string_ll_with_items(ll, d, indent);                         \
    unsigned nb_chars_indent = indent * 4;                                     \
    char *str = calloc(nb_chars + 1, sizeof(char));                            \
    if (!str)                                                                  \
    {                                                                          \
        destroy_linked_list(ll);                                               \
        return NULL_STRING;                                                    \
    }                                                                          \
    /* |-> Start building the string */                                        \
    str[0] = '{';                                                              \
    str[1] = '\n';                                                             \
    unsigned insert_idx = 2;                                                   \
    char is_key = 1;                                                           \
    string_link_t *link = ll->head;                                            \
    while (link)                                                               \
    {                                                                          \
        if (is_key)                                                            \
        {                                                                      \
            /* Tabs */                                                         \
            memset(str + insert_idx, ' ', nb_chars_indent);                    \
            insert_idx += nb_chars_indent;                                     \
            str[insert_idx++] = '"';                                           \
            /* String's contents */                                            \
            memcpy(str + insert_idx, link->s.str, link->s.len);                \
            insert_idx += link->s.len;                                         \
            memcpy(str + insert_idx, "\": ", 3);                               \
            insert_idx += 3;                                                   \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            if (link->is_from_str)                                             \
            {                                                                  \
                str[insert_idx++] = '"';                                       \
            }                                                                  \
            memcpy(str + insert_idx, link->s.str, link->s.len);                \
            insert_idx += link->s.len;                                         \
            if (link->is_from_str)                                             \
            {                                                                  \
                str[insert_idx++] = '"';                                       \
            }                                                                  \
            /* Comma and line return */                                        \
            if (link->next)                                                    \
            {                                                                  \
                str[insert_idx++] = ',';                                       \
            }                                                                  \
            str[insert_idx++] = '\n';                                          \
        }                                                                      \
        is_key = !is_key;                                                      \
        link = link->next;                                                     \
    }                                                                          \
    if (indent == 1)                                                           \
    {                                                                          \
        str[nb_chars - 2] = '}';                                               \
        str[nb_chars - 1] = '\n';                                              \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        /* Tabs before the last '}' */                                         \
        memset(str + insert_idx, ' ', nb_chars_indent - 4);                    \
        insert_idx += nb_chars_indent - 4;                                     \
        str[nb_chars - 1] = '}';                                               \
    }                                                                          \
    /* |-> End of string building */                                           \
    destroy_linked_list(ll);                                                   \
    return STRING_OF(str, nb_chars)

#define WRITE_JSON_TO_FILE(get_rx_array_as_str, get_rx_dict_as_str)            \
    if (!j || !file_name)                                                      \
    {                                                                          \
        return;                                                                \
    }                                                                          \
    FILE *f = fopen(file_name, "w");                                           \
    if (!f)                                                                    \
    {                                                                          \
        return;                                                                \
    }                                                                          \
    string_t s = IS_ARRAY(j) ? get_rx_array_as_str(j->array, 1)                \
                             : get_rx_dict_as_str(j->dict, 1);                 \
    if (!s.str)                                                                \
    {                                                                          \
        fclose(f);                                                             \
        return;                                                                \
    }                                                                          \
    printf("%s", s.str);                                                       \
    fwrite(s.str, sizeof(char), s.len, f);                                     \
    free(s.str);                                                               \
    fclose(f)

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
string_t get_long_as_str(long value);
string_t get_double_as_str(double value);
string_t get_exp_long_as_str(exponent_long_t value);
string_t get_exp_double_as_str(exponent_double_t value);
string_t get_bool_as_str(char value);
string_t get_null_as_str();

void add_link(string_linked_list_t *ll, string_t str, char str_needs_free,
              char is_from_str);
void destroy_linked_list(string_linked_list_t *ll);

#endif // !BASE_JSON_WRITE_H
