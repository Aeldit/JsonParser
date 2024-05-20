#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lists/linked_lists.h"

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
    struct states s = (struct states){ 0 };
    s.is_waiting_key = 1;

    ll_char_ctrl *llcc = init_ll();
    if (llcc == NULL)
    {
        return NULL;
    }

    // Possible elements
    char *key = NULL;
    char *str = NULL;
    /*long num = 0;
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
            ++s.is_in_json;
            break;
        case '}':
            --s.is_in_json;
            break;
        case '[':
            ++s.is_in_array;
            break;
        case ']':
            --s.is_in_array;
            break;
        case ':':
            if (!s.is_in_key)
            {
                s.is_waiting_value = 1;
            }
            break;
        case ',':
            if (!s.is_in_key)
            {
                s.is_waiting_key = 1;
            }
            break;

        // Special characters
        case '\n':
            if (!s.is_in_key)
            {
                continue;
            }
            break;
        case '\t':
            if (!s.is_in_key)
            {
                continue;
            }
            break;
        case ' ':
            if (!s.is_in_key)
            {
                continue;
            }
            break;

        case '"':
            if (prev_c != '\\')
            {
                if (!s.is_in_str)
                {
                    s.is_in_str = 1;
                }

                if (s.is_in_key)
                {
                    s.is_in_key = 0;
                    key = get_final_string(llcc);
                    printf("key : %s\n", key);
                }
                else if (s.is_in_value)
                {
                    s.is_in_value = 0;
                    str = get_final_string(llcc);
                    printf("str : %s\n", str);
                    add_str(jd, key, str);
                }
                // Prepare key acquisition
                else if (s.is_waiting_key)
                {
                    s.is_waiting_key = 0;
                    s.is_in_key = 1;
                }
                // Prepare value acquisition
                else if (s.is_waiting_value)
                {
                    s.is_waiting_value = 0;
                    s.is_in_value = 1;
                }
            }
            break;

        default:
            if (s.is_in_str)
            {
                add_char_to_ll(llcc, c);
            }
            break;
        }
        prev_c = c;
    }

    destroy_llcc(llcc);
    puts("\n");
    print_json(jd->pairs);
    destroy_dict(jd);
    return jd;
}
