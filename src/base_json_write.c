#include "base_json_write.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
void add_link(string_linked_list_t *ll, string_t str, char str_needs_free,
              char is_from_str)
{
    if (!ll)
    {
        return;
    }

    string_link_t *sl = calloc(1, sizeof(string_link_t));
    if (!sl)
    {
        return;
    }
    sl->s = str;
    sl->s_needs_free = str_needs_free;
    sl->is_from_str = is_from_str;

    if (!ll->head)
    {
        ll->head = sl;
        ll->tail = sl;
    }
    else if (!ll->tail)
    {
        free(sl);
    }
    else
    {
        ll->tail->next = sl;
        ll->tail = sl;
    }
}

void destroy_linked_list(string_linked_list_t *ll)
{
    if (!ll)
    {
        return;
    }

    string_link_t *link = ll->head;
    while (link)
    {
        string_link_t *tmp = link;
        link = link->next;
        if (tmp->s_needs_free)
        {
            free(tmp->s.str);
        }
        free(tmp);
    }
    free(ll);
}

/*******************************************************************************
**                                GETS AS STR                                 **
*******************************************************************************/
string_t get_int_as_str(int value)
{
    char is_neg = value < 0;
    unsigned nb_chars = is_neg ? 2 : 1;
    if (is_neg)
    {
        value *= -1;
    }

    int tmp_value = value;
    while (tmp_value /= 10)
    {
        ++nb_chars;
    }

    char *str = calloc(nb_chars + 1, sizeof(char));
    if (!str)
    {
        return EMPTY_STRING;
    }

    string_t s = STRING_OF(str, nb_chars);

    while (nb_chars)
    {
        str[--nb_chars] = value % 10 + '0';
        value /= 10;
    }

    if (is_neg)
    {
        str[0] = '-';
    }
    return s;
}

string_t get_double_as_str(double value)
{
    // 18 : 10 int digits + '.' + 6 floating point digits + '\0'
    char double_str[18];
    snprintf(double_str, 18, "%lf", value);

    int nb_chars = 0;
    int nb_decimals = 6;
    char is_in_decimals = 1;
    char non_zero_decimal_found = 0;
    for (int i = 17; i >= 0; --i)
    {
        char s = double_str[i];
        if (s == '.')
        {
            is_in_decimals = 0;
        }
        if (is_in_decimals && '1' <= s && s <= '9')
        {
            non_zero_decimal_found = 1;
        }
        if (s == '0' && is_in_decimals && !non_zero_decimal_found)
        {
            --nb_decimals;
        }
        if (!is_in_decimals)
        {
            ++nb_chars;
        }
    }

    char *str = calloc(18, sizeof(char));
    if (!str)
    {
        return EMPTY_STRING;
    }

    string_t s = STRING_OF(str, nb_chars + nb_decimals - (nb_decimals ? 0 : 1));
    memcpy(str, double_str, s.length);
    return s;
}

string_t get_bool_as_str(char value)
{
    char nb_chars = (value ? 4 : 5);
    char *str = calloc(nb_chars + 1, sizeof(char));
    memcpy(str, value ? "true" : "false", nb_chars);
    return STRING_OF(str ? str : 0, str ? nb_chars : 0);
}

string_t get_null_as_str()
{
    char *str = calloc(5, sizeof(char));
    if (!str)
    {
        return EMPTY_STRING;
    }
    memcpy(str, "null", 4);
    return STRING_OF(str, 4);
}
