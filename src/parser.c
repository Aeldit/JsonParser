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
    if (jd == NULL || key == NULL || value == NULL)
    {
        return;
    }
    size_t len = strlen(value);

    // Booleans
    char is_boolean = is_str_boolean(value, len);
    if (is_boolean == 1) // true
    {
        add_bool(jd, key, 1);
    }
    else if (is_boolean == 2) // false
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

size_t get_array_size(char *str, size_t len)
{
    if (str == NULL)
    {
        return 0;
    }

    size_t size = 0;
    unsigned is_in_array_brackets = 0;
    // Counts the number of ',' in the array that are not inside a string
    for (size_t i = 0; i < len; ++i)
    {
        if (str[i] == '[')
        {
            ++is_in_array_brackets;
        }
        else if (str[i] == ']')
        {
            --is_in_array_brackets;
        }
        else if (str[i] == ',' && !is_in_array_brackets)
        {
            ++size;
        }
    }
    // The last element doesn't have a ',' after, so we add 1 manually
    if (size != 0)
    {
        ++size;
    }
    printf("size : %lu\n", size);
    return size;
}

size_t get_array_buff_size(size_t offset, FILE *f)
{
    if (f == NULL)
    {
        return 0;
    }

    size_t initial_offset = offset;

    if (fseek(f, offset++, SEEK_SET) != 0)
    {
        return 0;
    }

    size_t nb_array_brackets = 0;
    char is_in_str = 0;
    char c = '\0';
    char prev_c = '\0';
    while ((c = fgetc(f)))
    {
        if (fseek(f, offset++, SEEK_SET) != 0)
        {
            break;
        }

        if (is_in_str && (c != '"' && prev_c != '\\'))
        {
            prev_c = c;
            continue;
        }

        switch (c)
        {
        case '[':
            ++nb_array_brackets;
            break;
        case ']':
            if (nb_array_brackets == 0)
            {
                return offset - 2 - initial_offset;
            }
            --nb_array_brackets;
            break;
        case '"':
            if (prev_c != '\\')
            {
                is_in_str = !is_in_str;
            }
            break;
        }
        prev_c = c;
    }
    return 0;
}

size_t parse_nested_array(json_dict_st *jd, json_array_st *nested_ja,
                          char *buff, size_t buff_size, size_t pos)
{
    if (nested_ja == NULL)
    {
        return 0;
    }

    ll_char_ctrl_st *llcc = init_ll();
    if (llcc == NULL)
    {
        return 0;
    }

    char is_in_str = 0;
    char is_str = 0; // Stores whether the previous value was a string
    // char is_in_nested_array = 0;
    char prev_c = '\0';
    for (; pos < buff_size; ++pos)
    {
        printf("at pos : %c\n", buff[pos]);
        // If we are in a string
        if (is_in_str && (buff[pos] != '"' && prev_c != '\\'))
        {
            add_char_to_ll(llcc, buff[pos]);
            continue;
        }

        switch (buff[pos])
        {
        case ']':
            if (is_str)
            {
                is_str = 0;
                add_str_to_array(jd, nested_ja, get_final_string(llcc));
            }
            else
            {
                char *value = get_final_string(llcc);
                if (value == NULL)
                {
                    continue;
                }
                size_t len = strlen(value);

                char is_boolean = is_str_boolean(value, len);
                if (is_boolean)
                {
                    add_bool_to_array(jd, nested_ja, is_boolean == 1 ? 1 : 0);
                }
                else
                {
                    if (is_str_number(value, len))
                    {
                        add_num_to_array(jd, nested_ja,
                                         str_to_long(value, len));
                    }
                    else
                    {
                        add_array_to_array(jd, nested_ja, nested_ja);
                    }
                }
                free(value);
            }
            return pos;
        case ',':
            if (is_str)
            {
                is_str = 0;
                add_str_to_array(jd, nested_ja, get_final_string(llcc));
            }
            else
            {
                char *value = get_final_string(llcc);
                if (value == NULL)
                {
                    continue;
                }
                size_t len = strlen(value);

                char is_boolean = is_str_boolean(value, len);
                if (is_boolean)
                {
                    add_bool_to_array(jd, nested_ja, is_boolean == 1 ? 1 : 0);
                }
                else
                {
                    if (is_str_number(value, len))
                    {
                        add_num_to_array(jd, nested_ja,
                                         str_to_long(value, len));
                    }
                    else
                    {
                        add_array_to_array(jd, nested_ja, nested_ja);
                    }
                }
                free(value);
            }
            break;
        case '"':
            if (prev_c != '\\')
            {
                is_in_str = !is_in_str;
                is_str = 1;
            }
            break;
        }
    }
    return pos;
}

void parse_array(json_dict_st *jd, char *buff, size_t buff_size, char *key)
{
    if (jd == NULL || buff == NULL || key == NULL || buff_size == 0)
    {
        return;
    }
    size_t size = get_array_size(buff, buff_size);

    json_array_st *ja = array_init(size);
    if (ja == NULL)
    {
        return;
    }

    ll_char_ctrl_st *llcc = init_ll();
    if (llcc == NULL)
    {
        free(ja);
        return;
    }

    json_array_st *nested_ja;
    char is_in_str = 0;
    char prev_c = '\0';
    char is_str = 0;
    // <= because we add 1 because of the '\0'
    for (size_t i = 0; i <= buff_size; ++i)
    {
        // End of the string
        if (i == buff_size)
        {
            if (is_str)
            {
                is_str = 0;
                add_str_to_array(jd, ja, get_final_string(llcc));
            }
            else
            {
                char *value = get_final_string(llcc);
                if (value == NULL)
                {
                    continue;
                }
                size_t len = strlen(value);

                char is_boolean = is_str_boolean(value, len);
                if (is_boolean)
                {
                    add_bool_to_array(jd, ja, is_boolean == 1 ? 1 : 0);
                }
                else
                {
                    if (is_str_number(value, len))
                    {
                        add_num_to_array(jd, ja, str_to_long(value, len));
                    }
                }
                free(value);
            }
            continue;
        }

        // If in a str and not a '"'
        if (is_in_str && (buff[i] != '"' && prev_c != '\\'))
        {
            add_char_to_ll(llcc, buff[i]);
            continue;
        }

        switch (buff[i])
        {
        case '[':
            nested_ja = calloc(1, sizeof(json_array_st));
            if (nested_ja == NULL)
            {
                return;
            }
            printf("i = %lu\n", i);
            i += parse_nested_array(jd, nested_ja, buff, buff_size, i + 1) + 1;
            printf("i = %lu %d\n", i, buff[i]);
            break;
        case ',':
            if (is_str)
            {
                is_str = 0;
                add_str_to_array(jd, ja, get_final_string(llcc));
            }
            else
            {
                char *value = get_final_string(llcc);
                if (value == NULL)
                {
                    continue;
                }
                size_t len = strlen(value);

                char is_boolean = is_str_boolean(value, len);
                if (is_boolean)
                {
                    add_bool_to_array(jd, ja, is_boolean == 1 ? 1 : 0);
                }
                else
                {
                    if (is_str_number(value, len))
                    {
                        add_num_to_array(jd, ja, str_to_long(value, len));
                    }
                    else
                    {
                        add_array_to_array(jd, ja, nested_ja);
                    }
                }
                free(value);
            }
            break;

        case ' ':
            break;
        case '\n':
            break;
        case '\t':
            break;

        case '"':
            if (prev_c != '\\')
            {
                is_in_str = !is_in_str;
                is_str = 1;
            }
            break;
        default:
            add_char_to_ll(llcc, buff[i]);
            break;
        }
        prev_c = buff[i];
    }
    add_array(jd, key, ja);
    destroy_llcc(llcc);
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

    size_t offset = 0;
    if (fseek(f, offset++, SEEK_SET) != 0)
    {
        fclose(f);
        return NULL;
    }

    json_dict_st *jd = init_dict();
    if (jd == NULL)
    {
        fclose(f);
        return NULL;
    }
    struct states s = (struct states){ 0 };
    s.is_waiting_key = 1;

    ll_char_ctrl_st *llcc = init_ll();
    if (llcc == NULL)
    {
        destroy_dict(jd);
        return NULL;
    }

    char *key = NULL;

    size_t array_buff_size = 0;
    size_t tmp_offset = 0;

    char c = '\0';
    char prev_c = '\0';
    while ((c = fgetc(f)) != EOF)
    {
        if (fseek(f, offset++, SEEK_SET) != 0)
        {
            break;
        }

        if (s.is_in_str && (c != '"' && prev_c != '\\'))
        {
            add_char_to_ll(llcc, c);
            continue;
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
            tmp_offset = offset - 1;
            //  This is the final size of the buffer we are going to
            //  fill wit the array
            array_buff_size = get_array_buff_size(offset - 1, f);
            // + 1 because this skips the ',' so we are able to go to the
            // next pair directly
            offset += array_buff_size + 1;

            if (array_buff_size != 0)
            {
                // + 1 because we have to add '\0'
                char *array_buffer = calloc(array_buff_size + 1, sizeof(char));
                if (array_buffer == NULL)
                {
                    break;
                }

                for (size_t i = 0; i < array_buff_size; ++i)
                {
                    if (fseek(f, tmp_offset++, SEEK_SET) != 0)
                    {
                        break;
                    }
                    array_buffer[i] = fgetc(f);
                }
                array_buffer[array_buff_size] = '\0';

                parse_array(jd, array_buffer, array_buff_size, key);
                free(array_buffer);
                array_buff_size = 0;

                s.is_waiting_key = 1;
            }
            break;
        case ':':
            s.is_in_value = 1;
            break;
        case ',':
            if (s.is_in_value)
            {
                s.is_in_value = 0;
                handle_non_str_value(jd, key, get_final_string(llcc));
            }
            s.is_waiting_key = 1;
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
