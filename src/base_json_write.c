#include "base_json_write.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
**                               LOCAL FUNCTIONS                              **
*******************************************************************************/
unsigned get_nb_char_long(long n)
{
    unsigned nb_char = n < 0 ? 2 : 1;
    n *= n < 0 ? -1 : 1;
    while (n /= 10)
    {
        ++nb_char;
    }
    return nb_char;
}

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
string_t get_long_as_str(long value)
{
    unsigned nb_chars = get_nb_char_long(value);
    char *str = calloc(nb_chars + 1, sizeof(char));
    if (!str)
    {
        return NULL_STRING;
    }
    snprintf(str, nb_chars + 1, "%ld", value);
    return STRING_OF(str, nb_chars);
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
        return NULL_STRING;
    }

    unsigned len = nb_chars + nb_decimals - (nb_decimals ? 0 : 1);
    memcpy(str, double_str, len);
    return STRING_OF(str, len);
}

string_t get_exp_long_as_str(exponent_long_t value)
{
    long number = value.number;
    long exponent = value.exponent;

    unsigned nb_chars_number = get_nb_char_long(number);
    unsigned nb_chars_exponent = get_nb_char_long(exponent);

    long len = nb_chars_number + 1 + nb_chars_exponent;
    char *str = calloc(len + 1, sizeof(char));
    if (!str)
    {
        return NULL_STRING;
    }

    snprintf(str, nb_chars_number + 1, "%ld", number);
    str[nb_chars_number] = 'e';
    snprintf(str + nb_chars_number + 1, nb_chars_exponent + 1, "%ld", exponent);
    return STRING_OF(str, len);
}

union print
{
    double value;
    char str[sizeof(double)];
};

string_t get_exp_double_as_str(exponent_double_t value)
{
    double number = value.number;
    long exponent = value.exponent;

    // 18 : 10 int digits + '.' + 6 floating point digits + '\0'
    char double_str[18];
    snprintf(double_str, 18, "%lf", number);

    unsigned nb_chars = 0;
    unsigned nb_decimals = 6;
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
    unsigned exp_len = get_nb_char_long(exponent);

    char *str = calloc(18 + 1 + exp_len, sizeof(char));
    if (!str)
    {
        return NULL_STRING;
    }

    unsigned num_len = nb_chars + nb_decimals - (nb_decimals ? 0 : 1);
    memcpy(str, double_str, num_len);
    str[num_len] = 'e';
    snprintf(str + num_len + 1, exp_len + 1, "%ld", exponent);
    return STRING_OF(str, num_len + 1 + exp_len);
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
        return NULL_STRING;
    }
    memcpy(str, "null", 4);
    return STRING_OF(str, 4);
}
