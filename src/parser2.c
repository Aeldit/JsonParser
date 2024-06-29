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
/**
** \brief Reads the string at position 'pos' in the given file, and adds it to
**        the given dict
** \param pos The pos of the '"' that starts the string of which we are
**            currently acquiring the length
*/
size_t str_len(FILE *f, size_t pos)
{
    if (f == NULL)
    {
        return 0;
    }

    size_t size = 0;
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
char *handle_string(json_dict_st *jd, FILE *f, size_t *pos)
{
    if (jd == NULL || f == NULL || pos == NULL)
    {
        return NULL;
    }

    size_t len = str_len(f, *pos);
    printf("%lu\n", len);
    if (len == 0)
    {
        return NULL;
    }

    char *str = calloc(len + 1, sizeof(char));
    if (str == NULL)
    {
        return NULL;
    }
    size_t idx = 0;

    for (size_t i = 0; i < len; ++i)
    {
        if (fseek(f, (*pos)++, SEEK_SET) != 0)
        {
            break;
        }
        str[idx++] = fgetc(f);
    }
    ++(*pos);
    return str;
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

    char *key = NULL;
    char *str = NULL;

    char c = '\0';
    while ((c = fgetc(f)) != EOF)
    {
        switch (c)
        {
        case '{':
            ++s.is_in_json;
            break;
        case '}':
            --s.is_in_json;
            break;
        case '[':
            break;
        case ':':
            break;
        case ',':
            break;

        case '"':
            if (s.is_waiting_key)
            {
                key = handle_string(jd, f, &offset);
                s.is_waiting_key = 0;
            }
            else
            {
                str = handle_string(jd, f, &offset);
                add_str(jd, key, str);
            }
            break;
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
