#include "parser.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define IS_NUMBER_START(c) (('0' <= (c) && (c) <= '9') || (c) == '-')
#define IS_BOOL_START(c) ((c) == 't' || (c) == 'f')

#define IS_END_CHAR(c) ((c) == ',' || (c) == '\n' || (c) == ']' || (c) == '}')

#define TYPED_VALUE_OF(v, t) ((typed_value_st){ .value = (v), .type = (t) })
#define ITEM_OF(k, v, t) ((item_st){ .key = (k), .value = (v), .type = (t) })

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
/**
** \struct str_and_len_tuple
** \brief Used by the parse_number() function to return multiple informations
*/
struct str_and_len_tuple
{
    char *str;
    uint64_t len;
    char is_float;
    char has_exponent;
};

/*******************************************************************************
**                              LOCAL FUNCTIONS                               **
*******************************************************************************/
/**
** \brief Reads the string at position 'pos' in the given file, and returns it
** \param f The file stream
** \param pos The pos of the '"' that starts the string we are currently
**            acquiring
*/
char *parse_string(FILE *f, uint64_t *pos)
{
    if (f == NULL || pos == NULL)
    {
        return NULL;
    }

    uint64_t size = (*pos);
    char c = '\0';
    char prev_c = '\0';
    while ((c = fgetc(f)) != EOF)
    {
        if (c == '"' && prev_c != '\\')
        {
            break;
        }

        if (fseek(f, size++, SEEK_SET) != 0)
        {
            break;
        }
        prev_c = c;
    }

    uint64_t len = size - (*pos) - 1;
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
    ++(*pos); // Because otherwise, we end up reading the last '"' of the str
    return str;
}

/**
** \brief Takes sl's char array and transforms it into an int64_t.
**        If the number has an exponent, the exponent is parsed as well and the
**        number is elevated to that exponent
** \param sl A pointer to an StrAndLenTuple object
** \returns The 0 in case of error (or if the number was 0), the number
**          otherwise
*/
int64_t str_to_long(struct str_and_len_tuple *sl)
{
    char *str = sl->str;
    uint64_t len = sl->len;
    if (str == NULL || len == 0)
    {
        return 0;
    }

    int64_t res = 0;
    uint64_t exponent = 0;
    char is_negative = str[0] == '-' ? -1 : 1;
    char is_in_exponent = 0;
    for (uint64_t i = 0; i < len; ++i)
    {
        if (sl->has_exponent && (str[i] == 'e' || str[i] == 'E'))
        {
            is_in_exponent = 1;
        }
        else if ('0' <= str[i] && str[i] <= '9')
        {
            if (is_in_exponent)
            {
                exponent = exponent * 10 + str[i] - '0';
            }
            else
            {
                res = res * 10 + str[i] - '0';
            }
        }
    }
    return sl->has_exponent ? pow(res * is_negative, exponent)
                            : res * is_negative;
}

/**
** \brief Takes sl's char array and transforms it into a double.
**        If the number has an exponent, the exponent is parsed as well and the
**        number is elevated to that exponent
** \param sl A pointer to an StrAndLenTuple object
** \returns The 0 in case of error (or if the number was 0), the number
**          otherwise
*/
double str_to_double(struct str_and_len_tuple *sl)
{
    char *str = sl->str;
    uint64_t len = sl->len;
    if (str == NULL || len == 0)
    {
        return 0;
    }

    double res = 0; // Integer part
    double dot_res = 0; // Decimal part
    uint64_t exponent = 0; // Only used if sl->has_exponent() is true
    uint64_t nb_digits_dot = 1;
    // If the number is negative, this is set to -1 and the final res is
    // multiplied by it
    char is_negative = str[0] == '-' ? -1 : 1;
    char dot_reached = 0;
    char is_in_exponent = 0;
    for (uint64_t i = 0; i < len; ++i)
    {
        if (str[i] == '.')
        {
            dot_reached = 1;
        }
        else if (sl->has_exponent && (str[i] == 'e' || str[i] == 'E'))
        {
            is_in_exponent = 1;
        }
        else if ('0' <= str[i] && str[i] <= '9')
        {
            if (is_in_exponent)
            {
                exponent = exponent * 10 + str[i] - '0';
            }
            else if (dot_reached)
            {
                dot_res = dot_res * 10 + str[i] - '0';
                nb_digits_dot *= 10;
            }
            else
            {
                res = res * 10 + str[i] - '0';
            }
        }
    }
    return sl->has_exponent
        ? pow(is_negative * (res + (dot_res / nb_digits_dot)), exponent)
        : is_negative * (res + (dot_res / nb_digits_dot));
}

char is_float(char *str, uint64_t len)
{
    if (str == NULL)
    {
        return 0;
    }

    for (uint64_t i = 0; i < len; ++i)
    {
        if (str[i] == '.')
        {
            return 1;
        }
    }
    return 0;
}

char has_exponent(char *str, uint64_t len)
{
    if (str == NULL)
    {
        return 0;
    }

    for (uint64_t i = 0; i < len; ++i)
    {
        if (str[i] == 'e' || str[i] == 'E')
        {
            return 1;
        }
    }
    return 0;
}

/**
** \brief Reads the file from the given pos and returns an instance of the
**        StrAndLenTuple class containing the number as a char array, the length
**        of the char array and whether the number is a float and has an
**        exponent
** \param f The file stream
** \param pos The pos of the start of the number of which we are currently
**            acquiring the length
*/
struct str_and_len_tuple parse_number(FILE *f, uint64_t *pos)
{
    if (f == NULL || pos == NULL)
    {
        return (struct str_and_len_tuple){
            .str = NULL, .len = 0, .is_float = 0, .has_exponent = 0
        };
    }

    // Because we already read the first digit (or sign)
    --(*pos);

    // Obtains the length of the value
    uint64_t size = (*pos);
    if (fseek(f, size++, SEEK_SET) != 0)
    {
        return (struct str_and_len_tuple){
            .str = NULL, .len = 0, .is_float = 0, .has_exponent = 0
        };
    }

    char c = '\0';
    while ((c = fgetc(f)) != EOF)
    {
        if (IS_END_CHAR(c))
        {
            break;
        }
        if (fseek(f, size++, SEEK_SET) != 0)
        {
            break;
        }
    }

    uint64_t len = size - (*pos) - 1;
    if (len == 0)
    {
        return (struct str_and_len_tuple){
            .str = NULL, .len = 0, .is_float = 0, .has_exponent = 0
        };
    }

    // Puts the value in the form of a char array
    char *str = calloc(len + 1, sizeof(char));
    if (str == NULL)
    {
        return (struct str_and_len_tuple){
            .str = NULL, .len = 0, .is_float = 0, .has_exponent = 0
        };
    }

    for (uint64_t i = 0; i < len; ++i)
    {
        if (fseek(f, (*pos)++, SEEK_SET) != 0)
        {
            break;
        }
        str[i] = fgetc(f);
    }
    return (struct str_and_len_tuple){ .str = str,
                                       .len = len,
                                       .is_float = is_float(str, len),
                                       .has_exponent = has_exponent(str, len) };
}

/**
** \returns 5 if false, 4 if true, 0 otherwise
**/
uint64_t parse_boolean(FILE *f, uint64_t *pos)
{
    if (f == NULL || pos == NULL)
    {
        return 0;
    }

    // Because we already read the first character
    --(*pos);

    uint64_t size = (*pos);
    if (fseek(f, size++, SEEK_SET) != 0)
    {
        return 0;
    }

    char c = '\0';
    while ((c = fgetc(f)) != EOF)
    {
        if (IS_END_CHAR(c))
        {
            break;
        }
        if (fseek(f, size++, SEEK_SET) != 0)
        {
            break;
        }
    }
    uint64_t len = size - (*pos) - 1;

    (*pos) += len;
    return len;
}

/**
** \param f The file stream
** \param pos The pos of the character just after the '[' that begins the
**            current array
** \returns The number of elements of the current array
*/
uint64_t get_nb_elts_array(FILE *f, uint64_t pos)
{
    if (f == NULL)
    {
        return 0;
    }

    uint64_t offset = pos;
    if (fseek(f, offset++, SEEK_SET) != 0)
    {
        return 0;
    }

    uint64_t size = 0;

    char c = '\0';
    char prev_c = '\0';
    char is_in_array = 1;
    char is_in_dict = 0;
    char is_in_string = 0;
    char is_backslashing = 0;
    char comma_encountered = 0;
    while ((c = fgetc(f)) != EOF)
    {
        if (!is_in_array)
        {
            break;
        }

        if (c == '\\')
        {
            is_backslashing = !is_backslashing;
        }
        else if (!comma_encountered && c == ',' && is_in_array == 1)
        {
            comma_encountered = 1;
        }

        // If we are not in a string or if the string just ended
        if (!is_in_string || (is_in_string && c == '"' && !is_backslashing))
        {
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
                // Empty array
                if (is_in_array == 1 && prev_c == '\0')
                {
                    return 0;
                }
                --is_in_array;
            }
            else if (c == '{')
            {
                ++is_in_dict;
            }
            else if (c == '}')
            {
                --is_in_dict;
            }
            else if (!is_in_dict && is_in_array == 1 && c == ',')
            {
                ++size;
            }
        }

        if (fseek(f, offset++, SEEK_SET) != 0)
        {
            break;
        }

        if (c != ' ' && c != '\t' && c != '\n')
        {
            prev_c = c;
        }
    }
    if (size >= 1 && comma_encountered)
    {
        ++size;
    }
    // If there was only one value, there was no ',', so the element wasn't
    // detected
    if (size == 0 && prev_c != '\0')
    {
        ++size;
    }
    return size;
}

/**
** \param f The file stream
** \param pos The pos of the character just after the '{' that begins the
**            current dict
** \returns The number of elements of the current dict
*/
uint64_t get_nb_elts_dict(FILE *f, uint64_t pos)
{
    if (f == NULL)
    {
        return 0;
    }

    uint64_t offset = pos;
    if (fseek(f, offset++, SEEK_SET) != 0)
    {
        return 0;
    }

    uint64_t size = 0;
    // Used for the case where the dict contains only one element, and so does
    // not contain a ','
    uint64_t single_elt_found = 0;

    char c = '\0';
    char is_in_dict = 1;
    char is_in_array = 0;
    char is_in_string = 0;
    char is_backslashing = 0;
    while ((c = fgetc(f)) != EOF)
    {
        if (!is_in_dict)
        {
            break;
        }

        if (c == '\\')
        {
            is_backslashing = !is_backslashing;
        }

        // If we are not in a string or if the string just ended
        if (!is_in_string || (is_in_string && c == '"' && !is_backslashing))
        {
            if (c == '"')
            {
                is_in_string = !is_in_string;
                single_elt_found = 1;
            }
            else if (c == '[')
            {
                ++is_in_array;
            }
            else if (c == ']')
            {
                --is_in_array;
            }
            else if (c == '{')
            {
                ++is_in_dict;
            }
            else if (c == '}')
            {
                --is_in_dict;
            }
            else if (!is_in_array && is_in_dict == 1 && c == ',')
            {
                ++size;
            }
        }

        if (fseek(f, offset++, SEEK_SET) != 0)
        {
            break;
        }
    }
    return size == 0 ? single_elt_found : size + 1;
}

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
json_array_st *parse_array(storage_st *s, FILE *f, uint64_t *pos)
{
    if (f == NULL || pos == NULL)
    {
        return NULL;
    }

    uint64_t nb_elts = get_nb_elts_array(f, *pos);
    uint64_t nb_elts_parsed = 0;

    json_array_st *ja = calloc(1, sizeof(json_array_st));
    if (nb_elts == 0)
    {
        return ja;
    }

    if (fseek(f, (*pos)++, SEEK_SET) != 0)
    {
        return ja;
    }

    char c = '\0';
    while ((c = fgetc(f)) != EOF && nb_elts_parsed < nb_elts)
    {
        // If we are not in a string or if the string just ended
        if (c == '"')
        {
            char *value = parse_string(f, pos);
            char *stored = store_string(s, value);
            if (stored != NULL)
            {
                addValue(ja, TYPED_VALUE_OF(stored, TYPE_STR));
            }
            ++nb_elts_parsed;
        }
        else if (IS_NUMBER_START(c))
        {
            struct str_and_len_tuple sl = parse_number(f, pos);
            if (sl.str == NULL)
            {
                continue;
            }

            if (sl.is_float)
            {
                double value = str_to_double(&sl);
                double *stored = store_double(s, value);
                if (stored != NULL)
                {
                    addValue(ja, TYPED_VALUE_OF(stored, TYPE_DOUBLE));
                }
            }
            else
            {
                int64_t value = str_to_long(&sl);
                int64_t *stored = store_int(s, value);
                if (stored != NULL)
                {
                    addValue(ja, TYPED_VALUE_OF(stored, TYPE_INT));
                }
            }
            free(sl.str);
            ++nb_elts_parsed;
        }
        else if (IS_BOOL_START(c))
        {
            uint64_t len = parse_boolean(f, pos);
            if (len == 0 || (c == 'f' && len != 5) || (c == 't' && len != 4))
            {
                continue;
            }

            char value = len == 4 ? 1 : 0;
            char *stored = store_boolean(s, value);
            if (stored != NULL)
            {
                addValue(ja, TYPED_VALUE_OF(stored, TYPE_BOOL));
            }
            ++nb_elts_parsed;
        }
        else if (c == 'n')
        {
            addValue(ja, TYPED_VALUE_OF(NULL, TYPE_NULL));
            (*pos) += 3;
            ++nb_elts_parsed;
        }
        else if (c == '[')
        {
            json_array_st *value = parse_array(s, f, pos);
            json_array_st *stored = store_array(s, value);
            if (stored != NULL)
            {
                addValue(ja, TYPED_VALUE_OF(stored, TYPE_ARR));
            }
            ++nb_elts_parsed;
        }
        else if (c == '{')
        {
            json_dict_st *value = parse_json_dict(s, f, pos);
            json_dict_st *stored = store_dict(s, value);
            if (stored != NULL)
            {
                addValue(ja, TYPED_VALUE_OF(stored, TYPE_DICT));
            }
            ++nb_elts_parsed;
        }

        if (fseek(f, (*pos)++, SEEK_SET) != 0)
        {
            break;
        }
    }
    --(*pos);
    return ja;
}

json_dict_st *parse_json_dict(storage_st *s, FILE *f, uint64_t *pos)
{
    if (f == NULL || pos == NULL)
    {
        return NULL;
    }

    char *key = NULL;
    uint64_t nb_elts = get_nb_elts_dict(f, *pos);
    uint64_t nb_elts_parsed = 0;

    json_dict_st *jd = calloc(1, sizeof(json_dict_st));
    if (fseek(f, (*pos)++, SEEK_SET) != 0)
    {
        return jd;
    }

    char c = '\0';
    char is_waiting_key = 1;
    while ((c = fgetc(f)) != EOF && nb_elts_parsed < nb_elts)
    {
        if (c == '"')
        {
            if (is_waiting_key)
            {
                key = parse_string(f, pos);
                is_waiting_key = 0;
            }
            else
            {
                char *value = parse_string(f, pos);
                char *stored = store_string(s, value);
                if (stored != NULL)
                {
                    addItem(jd, ITEM_OF(key, stored, TYPE_STR));
                }
                ++nb_elts_parsed;
            }
        }
        else if (IS_NUMBER_START(c))
        {
            struct str_and_len_tuple sl = parse_number(f, pos);
            if (sl.str == NULL)
            {
                continue;
            }

            if (sl.is_float)
            {
                double value = str_to_double(&sl);
                double *stored = store_double(s, value);
                if (stored != NULL)
                {
                    addItem(jd, ITEM_OF(key, stored, TYPE_DOUBLE));
                }
            }
            else
            {
                int64_t value = str_to_long(&sl);
                int64_t *stored = store_int(s, value);
                if (stored != NULL)
                {
                    addItem(jd, ITEM_OF(key, stored, TYPE_INT));
                }
            }
            free(sl.str);
            ++nb_elts_parsed;
        }
        else if (IS_BOOL_START(c))
        {
            uint64_t len = parse_boolean(f, pos);
            if (len == 0 || (c == 'f' && len != 5) || (c == 't' && len != 4))
            {
                continue;
            }

            char value = len == 4 ? 1 : 0;
            char *stored = store_boolean(s, value);
            if (stored != NULL)
            {
                addItem(jd, ITEM_OF(key, stored, TYPE_BOOL));
            }
            ++nb_elts_parsed;
        }
        else if (c == 'n')
        {
            addItem(jd, ITEM_OF(key, NULL, TYPE_NULL));
            (*pos) += 3;
            ++nb_elts_parsed;
        }
        else if (c == '[')
        {
            json_array_st *value = parse_array(s, f, pos);
            json_array_st *stored = store_array(s, value);
            if (stored != NULL)
            {
                addItem(jd, ITEM_OF(key, stored, TYPE_ARR));
            }
            ++nb_elts_parsed;
        }
        else if (c == '{')
        {
            json_dict_st *value = parse_json_dict(s, f, pos);
            json_dict_st *stored = store_dict(s, value);
            if (stored != NULL)
            {
                addItem(jd, ITEM_OF(key, stored, TYPE_DICT));
            }
            ++nb_elts_parsed;
        }
        else if (c == ',')
        {
            is_waiting_key = 1;
        }

        if (fseek(f, (*pos)++, SEEK_SET) != 0)
        {
            break;
        }
    }
    --(*pos);
    return jd;
}
