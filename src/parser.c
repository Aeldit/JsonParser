#include "parser.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lists/linked_lists.h"

/*******************************************************************************
**                              LOCAL FUNCTIONS                               **
*******************************************************************************/
char is_str_number(char *str, size_t len)
{
    if (str == NULL || len == 0)
    {
        return 0;
    }

    for (size_t i = 0; i < len; ++i)
    {
        // If not a minus sign ('-') or a number, we return false
        if (str[i] != '-' && (str[i] < '0' || '9' < str[i]))
        {
            return 0;
        }
    }
    return 1;
}

/**
** \return 1 if the given string is "true", 2 if the given string is "false",
**         and 0 otherwise
*/
char is_str_boolean(char *str, size_t len)
{
    if (str == NULL || len < 4 || len > 5)
    {
        return 0;
    }

    if (len == 4 && str[0] == 't' && str[1] == 'r' && str[2] == 'u'
        && str[3] == 'e')
    {
        return 1;
    }
    if (len == 5 && str[0] == 'f' && str[1] == 'a' && str[2] == 'l'
        && str[3] == 's' && str[4] == 'e')
    {
        return 2;
    }
    return 0;
}

long str_to_long(char *str, size_t len)
{
    if (str == NULL || len == 0)
    {
        return 0;
    }
    long res = 0;
    char is_negative = 1;

    for (size_t i = 0; i < len; ++i)
    {
        if (str[i] == '-')
        {
            is_negative = -1;
        }
        else if ('0' <= str[i] && str[i] <= '9')
        {
            res = res * 10 + str[i] - '0';
        }
    }
    return res * is_negative;
}

void handle_non_str_value(json_dict_st *jd, char *key, char *value)
{
    size_t len = strlen(value);

    // Booleans
    char is_boolean = is_str_boolean(value, len);
    if (is_boolean == 1)
    {
        add_bool(jd, key, 1);
    }
    else if (is_boolean == 2)
    {
        add_bool(jd, key, 0);
    }
    else
    {
        if (is_str_number(value, len))
        {
            add_num(jd, key, str_to_long(value, len));
        }
    }
    free(value);
}

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
json_dict_st *parse(char *file)
{
    FILE *f = fopen(file, "r");
    if (f == NULL)
    {
        return NULL;
    }

    int offset = 0;
    if (fseek(f, offset++, SEEK_SET) != 0)
    {
        return NULL;
    }

    json_dict_st *jd = init_dict();
    if (jd == NULL)
    {
        return NULL;
    }
    struct states s = (struct states){ 0 };
    s.is_waiting_key = 1;

    ll_char_ctrl *llcc = init_ll();
    if (llcc == NULL)
    {
        return NULL;
    }

    char *key = NULL;

    char c = '\0';
    char prev_c = '\0';
    while ((c = fgetc(f)) != EOF)
    {
        if (fseek(f, offset++, SEEK_SET) != 0)
        {
            break;
        }

        switch (c)
        {
        case '{':
            if (!s.is_in_str)
            {
                ++s.is_in_json;
            }
            break;
        case '}':
            if (!s.is_in_str)
            {
                --s.is_in_json;
            }
            break;
        case '[':
            if (!s.is_in_str)
            {
                ++s.is_in_array;
            }
            break;
        case ']':
            if (!s.is_in_str)
            {
                --s.is_in_array;
            }
            break;
        case ':':
            if (!s.is_in_str)
            {
                s.is_in_value = 1;
            }
            break;
        case ',':
            if (!s.is_in_str)
            {
                if (s.is_in_value)
                {
                    s.is_in_value = 0;
                    handle_non_str_value(jd, key, get_final_string(llcc));
                }
                s.is_waiting_key = 1;
            }
            break;

        case '"':
            if (prev_c != '\\')
            {
                s.is_in_str = !s.is_in_str;

                if (s.is_in_key)
                {
                    s.is_in_key = 0;
                    key = get_final_string(llcc);
                }
                else if (!s.is_in_str && s.is_in_value)
                {
                    s.is_in_value = 0;
                    add_str(jd, key, get_final_string(llcc));
                }
                // Prepare key acquisition
                else if (s.is_waiting_key)
                {
                    s.is_waiting_key = 0;
                    s.is_in_key = 1;
                }
            }
            break;

        default:
            if (c == '\t' || c == ' ')
            {
                if (!s.is_in_str)
                {
                    continue;
                }
            }
            else if (c == '\n')
            {
                if (!s.is_in_str)
                {
                    // Last pair of the json object
                    if (s.is_in_value)
                    {
                        s.is_in_value = 0;
                        handle_non_str_value(jd, key, get_final_string(llcc));
                    }
                    continue;
                }
            }
            add_char_to_ll(llcc, c);
            break;
        }
        prev_c = c;
    }

    destroy_llcc(llcc);
    puts("");
    print_json(jd->pairs);
    destroy_dict(jd);
    return jd;
}
