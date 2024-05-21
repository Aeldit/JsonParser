#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lists/linked_lists.h"

void handle_non_str_value(json_dict_st *jd, char *key, char *value)
{
    printf("\n%s\n", value);
    // Booleans
    if (!strcmp(value, "true"))
    {
        add_bool(jd, key, 1);
    }
    else if (!strcmp(value, "false"))
    {
        add_bool(jd, key, 0);
    }
}

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

    // Possible elements
    char *key = NULL;
    /*char *str = NULL;
    long num = 0;
    json_dict_control_st *jc = NULL;
    list_control_st *l = NULL;
    char bool = 0;*/

    char c;
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
                    handle_non_str_value(jd, key, get_final_string(llcc));
                    s.is_in_value = 0;
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
                else if (s.is_in_value)
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
                    if (s.is_in_value)
                    {
                        handle_non_str_value(jd, key, get_final_string(llcc));
                        s.is_in_value = 0;
                    }
                    continue;
                }
            }
            printf("%c ", c);
            add_char_to_ll(llcc, c);
            break;
        }
        // printf("%c", c);
        prev_c = c;
    }

    destroy_llcc(llcc);
    puts("\n");
    print_json(jd->pairs);
    destroy_dict(jd);
    return jd;
}
