#include "parser.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lists/linked_lists.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define IS_NUMBER_START(c) (('0' <= (c) && (c) <= '9') || (c) == '-')
#define IS_BOOL_START(c) ((c) == 't' || (c) == 'f')

/*******************************************************************************
**                              LOCAL FUNCTIONS                               **
*******************************************************************************/
/**
** \brief Reads the string at position 'pos' in the given file, and returns its
**        size
** \param pos The pos of the '"' that starts the string of which we are
**            currently acquiring the length
*/
uint64_t jstr_len(FILE *f, size_t pos)
{
    if (f == NULL)
    {
        return 0;
    }

    uint64_t size = 0;
    char c = '\0';
    char prev_c = '\0';
    while ((c = fgetc(f)) != EOF)
    {
        if (fseek(f, pos++, SEEK_SET) != 0)
        {
            break;
        }

        if (c == '"' && prev_c != '\\')
        {
            break;
        }
        else
        {
            ++size;
        }
        prev_c = c;
    }
    return size - 1;
}

/**
** \brief Reads the string at position 'pos' in the given file, and adds it to
**        the given dict
** \param pos The pos of the '"' that starts the string of which we are
**            currently acquiring the length
*/
char *parse_string(json_dict_st *jd, FILE *f, uint64_t *pos)
{
    if (jd == NULL || f == NULL || pos == NULL)
    {
        return NULL;
    }

    uint64_t len = jstr_len(f, *pos);
    if (len == 0)
    {
        return NULL;
    }

    char *str = calloc(len + 1, sizeof(char));
    if (str == NULL)
    {
        return NULL;
    }

    for (uint64_t i = 0; i < len; ++i)
    {
        if (fseek(f, (*pos)++, SEEK_SET) != 0)
        {
            break;
        }
        str[i] = fgetc(f);
    }
    ++(*pos); // Because otherwise, we endu up reading the last '"' of the str
    return str;
}

uint64_t jval_len(FILE *f, uint64_t pos)
{
    if (f == NULL)
    {
        return 0;
    }

    uint64_t size = pos;
    if (fseek(f, size++, SEEK_SET) != 0)
    {
        return 0;
    }

    char c = '\0';
    while ((c = fgetc(f)) != EOF)
    {
        if (c == ',' || c == '\n')
        {
            break;
        }

        if (fseek(f, size++, SEEK_SET) != 0)
        {
            break;
        }
    }
    return size - pos - 1;
}

int64_t str_to_long(char *str, uint64_t len)
{
    if (str == NULL || len == 0)
    {
        return 0;
    }
    int64_t res = 0;
    char is_negative = 1;

    for (uint64_t i = 0; i < len; ++i)
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

int64_t parse_number(json_dict_st *jd, FILE *f, uint64_t *pos)
{
    if (jd == NULL || f == NULL || pos == NULL)
    {
        return 0;
    }

    // Because we already read the first digit (or sign)
    --(*pos);

    uint64_t len = jval_len(f, *pos);
    if (len == 0)
    {
        return 0;
    }

    char *str = calloc(len + 1, sizeof(char));
    if (str == NULL)
    {
        return 0;
    }

    for (uint64_t i = 0; i < len; ++i)
    {
        if (fseek(f, (*pos)++, SEEK_SET) != 0)
        {
            break;
        }
        str[i] = fgetc(f);
    }
    int64_t res = str_to_long(str, len);
    free(str);
    return res;
}

char parse_boolean(json_dict_st *jd, FILE *f, uint64_t *pos)
{
    if (jd == NULL || f == NULL || pos == NULL)
    {
        return 0;
    }

    // Because we already read the first character
    --(*pos);

    uint64_t len = jval_len(f, *pos);
    (*pos) += len;
    if (len == 5)
    {
        return 0; // false
    }
    else if (len == 4)
    {
        return 1; // true
    }
    return 2;
}

uint64_t jarray_len(FILE *f, uint64_t pos)
{
    if (f == NULL)
    {
        return 0;
    }

    uint64_t size = pos;
    if (fseek(f, size++, SEEK_SET) != 0)
    {
        return 0;
    }

    char c = '\0';
    char is_in_array = 0;
    char is_in_string = 0;
    while ((c = fgetc(f)) != EOF)
    {
        if (!is_in_string && !is_in_array && (c == ',' || c == '\n'))
        {
            break;
        }

        if (c == '"')
        {
            is_in_string = !is_in_string;
        }
        else if (c == '[')
        {
            ++is_in_array;
        }
        else if (c == ']')
        {
            --is_in_array;
        }

        if (fseek(f, size++, SEEK_SET) != 0)
        {
            break;
        }
    }
    return size - pos - 1;
}

json_array_st *parse_array(json_dict_st *jd, FILE *f, uint64_t *pos)
{
    if (jd == NULL || f == NULL || pos == NULL)
    {
        return NULL;
    }

    // Because we already read the first character
    --(*pos);

    uint64_t len = jarray_len(f, *pos);
    printf("array len = %lu\n", len);
    (*pos) += len;
    return 0;
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

    uint64_t offset = 0;
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

    char *key = NULL;
    char *str = NULL;

    char c = '\0';
    while ((c = fgetc(f)) != EOF)
    {
        if (c == '"')
        {
            if (s.is_waiting_key)
            {
                key = parse_string(jd, f, &offset);
                s.is_waiting_key = 0;
            }
            else
            {
                str = parse_string(jd, f, &offset);
                add_str(jd, key, str);
            }
        }
        else if (c == ',')
        {
            s.is_waiting_key = 1;
        }
        else if (c == '{')
        {
            ++s.is_in_json;
        }
        else if (c == '}')
        {
            --s.is_in_json;
        }
        else if (c == '[')
        {
            parse_array(jd, f, &offset);
        }
        else if (IS_NUMBER_START(c))
        {
            add_num(jd, key, parse_number(jd, f, &offset));
        }
        else if (IS_BOOL_START(c))
        {
            char bool = parse_boolean(jd, f, &offset);
            if (bool < 2)
            {
                add_bool(jd, key, bool);
            }
        }

        if (fseek(f, offset++, SEEK_SET) != 0)
        {
            break;
        }
    }

    puts("");
    print_json(jd->pairs);
    destroy_dict(jd);
    return jd;
}
