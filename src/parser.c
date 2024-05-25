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
        // Arrays
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
    // TODO: Handle nested arrays
    for (size_t i = 0; i < len; ++i)
    {
        if (str[i] == ',')
        {
            ++size;
        }
    }
    return size;
}

void handle_array(json_dict_st *jd, char *key, char *array)
{
    if (jd == NULL || key == NULL || array == NULL)
    {
        return;
    }

    size_t len = strlen(array);
    printf("'%s'\n", array);
    size_t size = get_array_size(array, len);
    printf("%lu\n", size);

    free(array);
}

void parse_array(json_dict_st *jd, char *buff, size_t buff_size, char *key)
{
    printf("'%s'\n", buff);

    char is_in_str = 0;
    char prev_c = '\0';
    unsigned size = 0;

    // Counts the number of ',' in the array that are not inside a string
    for (size_t i = 0; i < buff_size; ++i)
    {
        switch (buff[i])
        {
        case ',':
            if (!is_in_str)
            {
                ++size;
            }
            break;
        case '"':
            if (prev_c != '\\')
            {
                is_in_str = !is_in_str;
            }
            break;
        default:
            break;
        }
        prev_c = buff[i];
    }
    // The last element doesn't have a ',' after, so we add 1 manually
    if (size != 0)
    {
        ++size;
    }
    prev_c = '\0';

    json_array_st *ja = array_init(size);
    if (ja == NULL)
    {
        return;
    }

    ll_char_ctrl *llcc = init_ll();
    if (llcc == NULL)
    {
        return;
    }

    char is_str = 0;
    is_in_str = 0;

    // <= because we add 1 because of the '\0'
    for (size_t i = 0; i <= buff_size; ++i)
    {
        switch (buff[i])
        {
        case ' ':
            break;
        case '\n':
            break;

        case ',':
            if (!is_in_str)
            {
                if (is_str)
                {
                    is_str = 0;
                    add_str_to_array(jd, ja, get_final_string(llcc));
                }
                else
                {
                    char *value = get_final_string(llcc);
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
                }
            }
            break;
        case '\0':
            if (is_str)
            {
                is_str = 0;
                add_str_to_array(jd, ja, get_final_string(llcc));
            }
            else
            {
                char *value = get_final_string(llcc);
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
            }
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
    array_print(ja);
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

    unsigned offset = 0;
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

    unsigned array_buff_size = 0;
    unsigned array_braces_count = 0;

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
                // If we are already inside an array, we add 1 to the number of
                // braces found so we can count to the correct ']'
                if (s.is_in_array)
                {
                    ++array_braces_count;
                }
                else
                {
                    s.is_in_array = 1;
                    array_buff_size = offset - 1;
                }
            }
            break;
        case ']':
            if (!s.is_in_str)
            {
                if (array_braces_count)
                {
                    --array_braces_count;
                }
                else
                {
                    if (array_buff_size != 0)
                    {
                        s.is_in_array = 0;

                        unsigned tmp_offset = array_buff_size;
                        // This is the final size of the buffer we are going to
                        // fill wit the array
                        array_buff_size = offset - 2 - array_buff_size;

                        char *array_buffer =
                            calloc(array_buff_size + 1, sizeof(char *));
                        if (array_buffer == NULL)
                        {
                            break;
                        }

                        for (unsigned i = 0; i < array_buff_size; ++i)
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
                    }
                }
            }
            break;
        case ':':
            if (!s.is_in_str)
            {
                s.is_in_value = 1;
            }
            break;
        case ',':
            /*if (!s.is_in_array)
            {
                s.is_in_value = 0;
                --s.is_in_array;
                handle_array(jd, key, get_final_string(llcc));
            }
            else*/
            if (!s.is_in_str)
            {
                if (s.is_in_value)
                {
                    s.is_in_value = 0;
                    // handle_non_str_value(jd, key, get_final_string(llcc));
                }
                s.is_waiting_key = 1;
            }
            // Case where we are inside an array
            else
            {
                add_char_to_ll(llcc, c);
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
            if (!s.is_in_array)
            {
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
                        if (s.is_in_value && !s.is_in_array)
                        {
                            s.is_in_value = 0;
                            // handle_non_str_value(jd, key,
                            // get_final_string(llcc));
                        }
                        continue;
                    }
                }
                add_char_to_ll(llcc, c);
            }
            break;
        }
        prev_c = c;
    }

    destroy_llcc(llcc);
    puts("");
    // print_json(jd->pairs);
    destroy_dict(jd);
    return jd;
}
