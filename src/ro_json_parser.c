#include "ro_json_parser.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/*******************************************************************************
**                                   MACROS                                   **
*******************************************************************************/
#define IS_NUMBER_START(c) (('0' <= (c) && (c) <= '9') || (c) == '-')
#define IS_BOOL_START(c) ((c) == 't' || (c) == 'f')

#define IS_END_CHAR(c)                                                         \
    ((c) == 0 || (c) == ',' || (c) == '\n' || (c) == ']' || (c) == '}')

#define IS_STRING_END(c) ((c) == 0 || ((c) == '"' && prev_c != '\\'))

#define IS_NOT_BOOLEAN(c, l)                                                   \
    ((l) == 0 || ((c) == 'f' && (l) != 5) || ((c) == 't' && (l) != 4))

/**
** \def Used by the functions that read the file using fseef() and fgetc()
**      inside while loops, it sets the cursor on the 'p' position,
**      increments 'p' and puts in 'c' the current character
*/
#define SEEK_AND_GET_CHAR(p) (!fseek(f, p++, SEEK_SET) && (c = fgetc(f)) != EOF)

#ifndef MAX_READ_BUFF_SIZE
#    define MAX_READ_BUFF_SIZE (1 << 30) // ~= ~ 1 GB
#endif

#ifndef MAX_NESTED_ARRAYS
#    define MAX_NESTED_ARRAYS 255
#endif

#ifndef MAX_NESTED_DICTS
#    define MAX_NESTED_DICTS 255
#endif

#define NULL_STR_AND_LEN_TUPLE ((StrAndLenTuple){ .str = 0, .len = 0 })

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
/**
** \typedef StrAndLenTuple
** \brief Used by the parse_number() function to return multiple informations
*/
typedef struct
{
    char *str;
    unsigned len;
    char is_float;
    char has_exponent;
} StrAndLenTuple;

/*******************************************************************************
**                           FUNCTIONS DECLARATIONS                           **
*******************************************************************************/
ro_dict_t *parse_dict_buff(char *b, unsigned long *pos);
ro_dict_t *parse_dict(FILE *f, unsigned long *pos);

/*******************************************************************************
**                              LOCAL FUNCTIONS                               **
*******************************************************************************/
/**
** \brief Takes sl's char ro_array_t and transforms it into an int64_t.
**        If the number has an exponent, the exponent is parsed as well and the
**        number is elevated to that exponent
** \param sl A pointer to an StrAndLenTuple object
** \returns The 0 in case of error (or if the number was 0), the number
**          otherwise
*/
long str_to_long(StrAndLenTuple *sl)
{
    if (!sl)
    {
        return 0;
    }

    char *str = sl->str;
    unsigned long len = sl->len;
    if (!str || len == 0)
    {
        return 0;
    }

    long res = 0;
    unsigned long exponent = 0;
    char is_negative = str[0] == '-' ? -1 : 1;
    char is_in_exponent = 0;
    char c = 0;
    for (unsigned long i = 0; i < len; ++i)
    {
        c = str[i];
        if (sl->has_exponent && (c == 'e' || c == 'E'))
        {
            is_in_exponent = 1;
        }
        else if ('0' <= c && c <= '9')
        {
            if (is_in_exponent)
            {
                exponent = exponent * 10 + c - '0';
            }
            else
            {
                res = res * 10 + c - '0';
            }
        }
    }
    return sl->has_exponent ? pow(res * is_negative, exponent)
                            : res * is_negative;
}

/**
** \brief Takes sl's char ro_array_t and transforms it into a double.
**        If the number has an exponent, the exponent is parsed as well and the
**        number is elevated to that exponent
** \param sl A pointer to an StrAndLenTuple object
** \returns The 0 in case of error (or if the number was 0), the number
**          otherwise
*/
double str_to_double(StrAndLenTuple *sl)
{
    if (!sl)
    {
        return 0;
    }

    char *str = sl->str;
    unsigned long len = sl->len;
    if (!str || len == 0)
    {
        return 0;
    }

    double res = 0; // Integer part
    double dot_res = 0; // Decimal part
    unsigned long exponent = 0; // Only used if sl->has_exponent() is true
    unsigned long nb_digits_dot = 1;
    // If the number is negative, this is set to -1 and the final res is
    // multiplied by it
    char is_negative = str[0] == '-' ? -1 : 1;
    char dot_reached = 0;
    char is_in_exponent = 0;
    char c = 0;
    for (unsigned long i = 0; i < len; ++i)
    {
        c = str[i];
        if (c == '.')
        {
            dot_reached = 1;
        }
        else if (sl->has_exponent && (c == 'e' || c == 'E'))
        {
            is_in_exponent = 1;
        }
        else if ('0' <= c && c <= '9')
        {
            if (is_in_exponent)
            {
                exponent = exponent * 10 + c - '0';
            }
            else if (dot_reached)
            {
                dot_res = dot_res * 10 + c - '0';
                nb_digits_dot *= 10;
            }
            else
            {
                res = res * 10 + c - '0';
            }
        }
    }
    return sl->has_exponent
        ? pow(is_negative * (res + (dot_res / nb_digits_dot)), exponent)
        : is_negative * (res + (dot_res / nb_digits_dot));
}

char is_float(char *str, unsigned long len)
{
    if (!str)
    {
        return 0;
    }

    for (unsigned long i = 0; i < len; ++i)
    {
        if (str[i] == '.')
        {
            return 1;
        }
    }
    return 0;
}

char has_exponent(char *str, unsigned long len)
{
    if (!str)
    {
        return 0;
    }

    for (unsigned long i = 0; i < len; ++i)
    {
        if (str[i] == 'e' || str[i] == 'E')
        {
            return 1;
        }
    }
    return 0;
}

char max_nested_arrays_reached(long is_in_array)
{
    if (is_in_array == MAX_NESTED_ARRAYS)
    {
#ifdef DEBUG
        printf("Max number of nested arrays reached, aborting "
               "parsing\n");
#endif
        return 1;
    }
    return 0;
}

char max_nested_dicts_reached(long is_in_dict)
{
    if (is_in_dict == MAX_NESTED_DICTS)
    {
#ifdef DEBUG
        printf("Max number of nested dicts reached, aborting "
               "parsing\n");
#endif
        return 1;
    }
    return 0;
}

/**
** \brief Parses the string starting at 'pos + 1' (first char after the '"')
** \param buff The buffer containing the current json file or object
** \param idx A pointer to the unsigned long containing the index of the '"'
**            that started the string we want to parse
** \returns An empty string in case of error, the parsed string otherwise
*/
string_t parse_string_buff(char *buff, unsigned long *idx)
{
    if (!buff || !idx)
    {
        return EMPTY_STRING;
    }

    unsigned long start_idx = *idx + 1;
    unsigned long len = 0;
    char c = 0;
    char prev_c = 0;
    // Counts the number of characters until the first one that is an 'end char'
    while (1)
    {
        c = buff[start_idx + len];
        if (IS_STRING_END(c))
        {
            break;
        }
        prev_c = c;
        ++len;
    }

    if (len == 0)
    {
        return EMPTY_STRING;
    }

    char *str = calloc(len + 1, sizeof(char));
    if (!str)
    {
        return EMPTY_STRING;
    }
    memcpy(str, buff + start_idx, len);

    // + 1 to not read the last '"' when returning in the calling function
    *idx += len + 1;
    return (string_t){ .str = str, .length = len };
}

/**
** \brief Reads the buffer from the given pos - 1
** \param buff The buffer containing the current json file or object
** \param idx The index of the second character of the number (the first one
**            was already read)
** \returns An instance of the StrAndLenTuple class containing the number as a
**          char ro_array_t, the length of the char array and whether the number
*is
*a
**          float and has an exponent
*/
StrAndLenTuple parse_number_buff(char *buff, unsigned long *idx)
{
    if (!buff || !idx)
    {
        return NULL_STR_AND_LEN_TUPLE;
    }

    // Counts the number of characters until the first one that is an 'end char'
    unsigned long end_idx = *idx;
    unsigned long initial_i = end_idx;
    char c = 0;
    while (1)
    {
        c = buff[end_idx];
        if (IS_END_CHAR(c))
        {
            break;
        }
        ++end_idx;
    }

    // Number of chars
    unsigned long len = end_idx - initial_i;
    if (len == 0)
    {
        return NULL_STR_AND_LEN_TUPLE;
    }

    // Puts the value in the form of a char ro_array_t
    char *str = calloc(len + 1, sizeof(char));
    if (!str)
    {
        return NULL_STR_AND_LEN_TUPLE;
    }
    memcpy(str, buff + initial_i, len);

    *idx += len - 1;
    return (StrAndLenTuple){ .str = str,
                             .len = len,
                             .is_float = is_float(str, len),
                             .has_exponent = has_exponent(str, len) };
}

/**
** \returns 5 if false, 4 if true, 0 otherwise
**/
unsigned long parse_boolean_buff(char *buff, unsigned long *idx)
{
    if (!buff || !idx)
    {
        return 0;
    }

    unsigned long end_idx = *idx;
    char c = 0;
    while (1)
    {
        c = buff[end_idx];
        if (IS_END_CHAR(c))
        {
            break;
        }
        ++end_idx;
    }
    unsigned long len = end_idx - *idx;
    *idx += len - 1;
    return len;
}

/**
** \param buff The buffer containing the object currently being parsed
** \param idx The index of the character just after the '[' that begins the
**            current ro_array_t
** \returns The number of elements of the current ro_array_t
*/
unsigned long get_nb_elts_array_buff(char *buff, unsigned long idx)
{
    if (!buff || buff[idx] == ']')
    {
        return 0;
    }

    unsigned long nb_elts = 0;
    long is_in_array = 1;
    long is_in_dict = 0;
    char c = 0;
    char prev_c = 0;
    char is_in_string = 0;
    char is_backslashing = 0;
    char comma_encountered = 0;
    while (1)
    {
        c = buff[idx];
        if (!is_in_array || c == 0)
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
                if (max_nested_arrays_reached(is_in_array))
                {
                    return 0;
                }
                ++is_in_array;
            }
            else if (c == ']')
            {
                --is_in_array;
            }
            else if (c == '{')
            {
                if (max_nested_dicts_reached(is_in_dict))
                {
                    return 0;
                }
                ++is_in_dict;
            }
            else if (c == '}')
            {
                --is_in_dict;
            }
            else if (!is_in_dict && is_in_array == 1 && c == ',')
            {
                ++nb_elts;
            }
        }
        ++idx;

        if (c != ' ' && c != '\t' && c != '\n')
        {
            prev_c = c;
        }
    }

    // If there was only one value, there was no ',', so the element wasn't
    // detected or if at least one element was found, it means that a ','
    // was found
    if ((nb_elts == 0 && prev_c != 0) || (nb_elts >= 1 && comma_encountered))
    {
        ++nb_elts;
    }
    return nb_elts;
}

/**
** \param buff The buffer containing the object currently being parsed
** \param idx The index of the character just after the '{' that begins the
**            current dict
** \returns The number of elements of the current dict
*/
unsigned long get_nb_elts_dict_buff(char *buff, unsigned long idx)
{
    if (!buff || idx >= MAX_READ_BUFF_SIZE || buff[idx] == '}')
    {
        return 0;
    }

    unsigned long nb_elts = 0;
    // Used for the case where the dict contains only one element, and so
    // does not contain a ','
    unsigned long single_elt_found = 0;

    long is_in_dict = 1;
    long is_in_array = 0;
    char is_in_string = 0;
    char is_backslashing = 0;
    char c = 0;
    while (1)
    {
        c = buff[idx];
        if (!is_in_dict || c == 0)
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
                if (max_nested_arrays_reached(is_in_array))
                {
                    return 0;
                }
                ++is_in_array;
            }
            else if (c == ']')
            {
                --is_in_array;
            }
            else if (c == '{')
            {
                if (max_nested_dicts_reached(is_in_dict))
                {
                    return 0;
                }
                ++is_in_dict;
            }
            else if (c == '}')
            {
                --is_in_dict;
            }
            else if (!is_in_array && is_in_dict == 1 && c == ',')
            {
                ++nb_elts;
            }
        }
        ++idx;
    }
    return nb_elts == 0 ? single_elt_found : nb_elts + 1;
}

/**
** \param buff The buffer containing the object currently being parsed
** \param idx The index of the character '[' that begins the current ro_array_t
** \returns The json ro_array_t parsed from the position
*/
ro_array_t *parse_array_buff(char *b, unsigned long *idx)
{
    if (!b)
    {
        return 0;
    }

    unsigned long i = idx == 0 ? 0 : *idx + 1;

    unsigned long nb_elts_parsed = 0;
    unsigned long nb_elts = get_nb_elts_array_buff(b, i);

    ro_array_t *a = init_ro_array(nb_elts);
    if (!a || nb_elts == 0)
    {
        return a;
    }

    char c = 0;
    // We start at 1 because if we entered this function, it means that we
    // already read a '['
    unsigned long initial_i = i;
    while (1)
    {
        c = b[i];
        if (c == 0 || nb_elts_parsed >= nb_elts)
        {
            break;
        }

        if (c == '"')
        {
            ro_array_add_str(a, parse_string_buff(b, &i));
            ++nb_elts_parsed;
        }
        else if (IS_NUMBER_START(c))
        {
            StrAndLenTuple sl = parse_number_buff(b, &i);
            if (!sl.str)
            {
                continue;
            }

            if (sl.is_float)
            {
                ro_array_add_double(a, str_to_double(&sl));
            }
            else
            {
                ro_array_add_int(a, str_to_long(&sl));
            }
            free(sl.str);
            ++nb_elts_parsed;
        }
        else if (IS_BOOL_START(c))
        {
            unsigned long len = parse_boolean_buff(b, &i);
            if (IS_NOT_BOOLEAN(c, len))
            {
                continue;
            }
            ro_array_add_bool(a, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
        }
        else if (c == 'n')
        {
            ro_array_add_null(a);
            i += 3;
            ++nb_elts_parsed;
        }
        else if (c == '[')
        {
            ro_array_t *tmp_a = parse_array_buff(b, &i);
            if (!tmp_a)
            {
                break;
            }
            ro_array_add_array(a, tmp_a);
            ++nb_elts_parsed;
        }
        else if (c == '{')
        {
            ro_dict_t *tmp_jd = parse_dict_buff(b, &i);
            if (!tmp_jd)
            {
                break;
            }
            ro_array_add_dict(a, tmp_jd);
            ++nb_elts_parsed;
        }
        ++i;
    }
    if (idx)
    {
        *idx += i - initial_i;
    }
    return a;
}

/**
** \param b The buffer containing the object currently being parsed
** \param idx A pointer to the index of the character '{' that begins the
**            current dict.
**            If the given pointer is a nullptr, it means that the buffer is
*a
**            new one created just before calling this function, meaning the
**            index starts at 0
** \returns The json dict parsed from the index
*/
ro_dict_t *parse_dict_buff(char *b, unsigned long *idx)
{
    if (!b)
    {
        return 0;
    }

    unsigned long i = idx == 0 ? 0 : *idx + 1;

    unsigned long nb_elts_parsed = 0;
    unsigned long nb_elts = get_nb_elts_dict_buff(b, i);

    ro_dict_t *d = init_ro_dict(nb_elts);
    if (!d || nb_elts == 0)
    {
        return d;
    }

    string_t key = EMPTY_STRING;
    char c = 0;
    char is_waiting_key = 1;
    // We start at 1 because if we entered this function, it means that we
    // already read a '{'
    unsigned long initial_i = i;
    while (1)
    {
        c = b[i];
        if (c == 0 || nb_elts_parsed >= nb_elts)
        {
            break;
        }

        if (c == '"')
        {
            if (is_waiting_key)
            {
                key = parse_string_buff(b, &i);
                is_waiting_key = 0;
            }
            else
            {
                ro_dict_add_str(d, key, parse_string_buff(b, &i));
                ++nb_elts_parsed;
            }
        }
        else if (IS_NUMBER_START(c))
        {
            StrAndLenTuple sl = parse_number_buff(b, &i);
            if (!sl.str)
            {
                continue;
            }

            if (sl.is_float)
            {
                ro_dict_add_double(d, key, str_to_double(&sl));
            }
            else
            {
                ro_dict_add_int(d, key, str_to_long(&sl));
            }
            free(sl.str);
            ++nb_elts_parsed;
        }
        else if (IS_BOOL_START(c))
        {
            unsigned long len = parse_boolean_buff(b, &i);
            if (IS_NOT_BOOLEAN(c, len))
            {
                continue;
            }
            ro_dict_add_bool(d, key, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
        }
        else if (c == 'n')
        {
            ro_dict_add_null(d, key);
            i += 3;
            ++nb_elts_parsed;
        }
        else if (c == '[')
        {
            ro_array_t *tmp_ja = parse_array_buff(b, &i);
            if (!tmp_ja)
            {
                break;
            }
            ro_dict_add_array(d, key, tmp_ja);
            ++nb_elts_parsed;
        }
        else if (c == '{')
        {
            ro_dict_t *tmp_jd = parse_dict_buff(b, &i);
            if (!tmp_jd)
            {
                break;
            }
            ro_dict_add_dict(d, key, tmp_jd);
            ++nb_elts_parsed;
        }
        else if (c == ',')
        {
            is_waiting_key = 1;
        }
        ++i;
    }
    if (idx)
    {
        *idx += i - initial_i;
    }
    return d;
}

/**
** \brief Parses the string starting at 'pos + 1' (first char after the '"')
** \param f The file stream
** \param pos A pointer to the unsigned long containing the position of the
*'"'
**            that started the string we want to parse
** \returns An empty string in case of error, the parsed string otherwise
*/
string_t parse_string(FILE *f, unsigned long *pos)
{
    if (!f || !pos)
    {
        return EMPTY_STRING;
    }

    unsigned long i = *pos;
    char c = 0;
    char prev_c = 0;
    while (SEEK_AND_GET_CHAR(i) && !IS_STRING_END(c))
    {
        prev_c = c;
    }

    unsigned len = i - i - *pos - 1;
    if (len == 0)
    {
        return EMPTY_STRING;
    }

    char *str = calloc(len + 1, sizeof(char));
    if (!str)
    {
        return EMPTY_STRING;
    }

    if (fseek(f, *pos, SEEK_SET) != 0)
    {
        free(str);
        return EMPTY_STRING;
    }
    fread(str, sizeof(char), len, f);

    // +1 to not read the last '"' when returning in the calling function
    *pos += len + 1;
    return (string_t){ .str = str, .length = len };
}

/**
** \brief Reads the file from the given pos and returns an instance of the
**        StrAndLenTuple class containing the number as a char ro_array_t, the
*length
**        of the char ro_array_t and whether the number is a float and has an
**        exponent
** \param f The file stream
** \param pos The pos of the start of the number of which we are currently
**            acquiring the length
*/
StrAndLenTuple parse_number(FILE *f, unsigned long *pos)
{
    if (!f || !pos)
    {
        return NULL_STR_AND_LEN_TUPLE;
    }

    // Obtains the length of the value
    // -1 because we already read the first digit (or sign)
    unsigned long end_pos = *pos - 1;

    char c = 0;
    // end_pos is incremented for each character found to be part of a
    // number
    while (SEEK_AND_GET_CHAR(end_pos) && !IS_END_CHAR(c))
    {
    }

    unsigned long len = end_pos - *pos;
    if (len == 0)
    {
        return NULL_STR_AND_LEN_TUPLE;
    }

    // Puts the value in the form of a char ro_array_t
    char *str = calloc(len + 1, sizeof(char));
    if (!str)
    {
        return NULL_STR_AND_LEN_TUPLE;
    }

    // If we couldn't set the pos in the file
    if (fseek(f, *pos - 1, SEEK_SET) != 0)
    {
        free(str);
        return NULL_STR_AND_LEN_TUPLE;
    }
    fread(str, sizeof(char), len, f);

    *pos += len - 1;
    return (StrAndLenTuple){ .str = str,
                             .len = len,
                             .is_float = is_float(str, len),
                             .has_exponent = has_exponent(str, len) };
}

/**
** \returns 5 if false, 4 if true, 0 otherwise
**/
unsigned long parse_boolean(FILE *f, unsigned long *pos)
{
    if (!f || !pos)
    {
        return 0;
    }

    // -1 because we already read the first character
    unsigned long end_pos = *pos - 1;

    char c = 0;
    // end_pos is incremented for each character found to be part of a
    // boolean
    while (SEEK_AND_GET_CHAR(end_pos) && !IS_END_CHAR(c))
    {
    }
    unsigned long len = end_pos - *pos;
    *pos += len - 1;
    return len;
}

/**
** \param f The file stream
** \param pos The position in the file of the character after the '[' that
**            begins the current ro_array_t
** \param err A pointer to a char that will be updated if an error occurs
**            (we return the number of chars, so we can't use a number to
**            indicate an error)
** \returns The total number of characters in the current ro_array_t - 1 (the
*first
**          '[' is not counted)
*/
unsigned long get_nb_chars_in_array(FILE *f, unsigned long pos)
{
    if (!f)
    {
        return 0;
    }

    unsigned long nb_chars = 0;

    long is_in_array = 1;
    long is_in_dict = 0;
    char is_in_string = 0;
    char is_backslashing = 0;

    char c = 0;
    while (SEEK_AND_GET_CHAR(pos))
    {
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
            }
            else if (c == '[')
            {
                if (max_nested_arrays_reached(is_in_array))
                {
                    return 0;
                }
                ++is_in_array;
            }
            else if (c == ']')
            {
                --is_in_array;
            }
            else if (c == '{')
            {
                if (max_nested_dicts_reached(is_in_dict))
                {
                    return 0;
                }
                ++is_in_dict;
            }
            else if (c == '}')
            {
                --is_in_dict;
            }
        }
        ++nb_chars;

        if (!is_in_array)
        {
            break;
        }
    }
    return nb_chars;
}

/**
** \param f The file stream
** \param pos The position in the file of the character just after the '['
*that
**            begins the current ro_array_t
** \returns The number of elements of the current ro_array_t
*/
unsigned long get_nb_elts_array(FILE *f, unsigned long pos)
{
    if (!f)
    {
        return 0;
    }

    unsigned long nb_elts = 0;

    long is_in_array = 1;
    long is_in_dict = 0;
    char is_in_string = 0;
    char is_backslashing = 0;
    char comma_encountered = 0;

    char c = 0;
    char prev_c = 0;
    while (SEEK_AND_GET_CHAR(pos))
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
                if (max_nested_arrays_reached(is_in_array))
                {
                    return 0;
                }
                ++is_in_array;
            }
            else if (c == ']')
            {
                // Empty ro_array_t
                // TODO: Remove this and put it at the top like in the
                // buffered version
                if (is_in_array == 1 && prev_c == 0)
                {
                    return 0;
                }
                --is_in_array;
            }
            else if (c == '{')
            {
                if (max_nested_dicts_reached(is_in_dict))
                {
                    return 0;
                }
                ++is_in_dict;
            }
            else if (c == '}')
            {
                --is_in_dict;
            }
            else if (!is_in_dict && is_in_array == 1 && c == ',')
            {
                ++nb_elts;
            }
        }

        if (c != ' ' && c != '\t' && c != '\n')
        {
            prev_c = c;
        }
    }

    // If there was only one value, there was no ',', so the element wasn't
    // detected or if at least one element was found, it means that a ','
    // was found
    if ((nb_elts == 0 && prev_c != 0) || (nb_elts >= 1 && comma_encountered))
    {
        ++nb_elts;
    }
    return nb_elts;
}

/**
** \param f The file stream
** \param pos The position in the file of the character after the '{' that
**            begins the current dict
** \returns The total number of characters in the current dict - 1 (the
**          first '{' is not counted)
*/
unsigned long get_nb_chars_in_dict(FILE *f, unsigned long pos)
{
    if (!f)
    {
        return 0;
    }

    unsigned long nb_chars = 0;

    long is_in_dict = 1;
    long is_in_array = 0;
    char is_in_string = 0;
    char is_backslashing = 0;

    char c = 0;
    while (SEEK_AND_GET_CHAR(pos))
    {
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
            }
            else if (c == '[')
            {
                if (max_nested_arrays_reached(is_in_array))
                {
                    return 0;
                }
                ++is_in_array;
            }
            else if (c == ']')
            {
                --is_in_array;
            }
            else if (c == '{')
            {
                if (max_nested_dicts_reached(is_in_dict))
                {
                    return 0;
                }
                ++is_in_dict;
            }
            else if (c == '}')
            {
                --is_in_dict;
            }
        }
        ++nb_chars;

        if (!is_in_dict)
        {
            break;
        }
    }
    return nb_chars;
}

/**
** \param f The file stream
** \param pos The position in the file of the character just after the '{'
**            that begins the current dict
** \returns The number of elements of the current dict
*/
unsigned long get_nb_elts_dict(FILE *f, unsigned long pos)
{
    if (!f)
    {
        return 0;
    }

    unsigned long nb_elts = 0;

    long is_in_dict = 1;
    long is_in_array = 0;
    char is_in_string = 0;
    char is_backslashing = 0;
    // Used for the case where the dict contains only one element, and so
    // does not contain a ','
    unsigned long single_elt_found = 0;

    char c = 0;
    while (SEEK_AND_GET_CHAR(pos))
    {
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
                if (max_nested_arrays_reached(is_in_array))
                {
                    return 0;
                }
                ++is_in_array;
            }
            else if (c == ']')
            {
                --is_in_array;
            }
            else if (c == '{')
            {
                if (max_nested_dicts_reached(is_in_dict))
                {
                    return 0;
                }
                ++is_in_dict;
            }
            else if (c == '}')
            {
                --is_in_dict;
            }
            else if (!is_in_array && is_in_dict == 1 && c == ',')
            {
                ++nb_elts;
            }
        }

        if (!is_in_dict)
        {
            break;
        }
    }
    return nb_elts == 0 ? single_elt_found : nb_elts + 1;
}

/**
** \param f The file stream
** \param pos The postion in the file of the character after the '[' that
**            begins the current ro_array_t
** \returns The json ro_array_t parsed from the position
*/
ro_array_t *parse_array(FILE *f, unsigned long *pos)
{
    if (!f || !pos)
    {
        return 0;
    }

    unsigned long i = *pos;

    unsigned long nb_elts_parsed = 0;
    unsigned long nb_elts = get_nb_elts_array(f, i);

    ro_array_t *a = init_ro_array(nb_elts);
    if (!a || nb_elts == 0)
    {
        ++(*pos);
        return a;
    }

    char c = 0;
    while (SEEK_AND_GET_CHAR(i) && nb_elts_parsed < nb_elts && c != 0)
    {
        // If we are not in a string or if the string just ended
        if (c == '"')
        {
            ro_array_add_str(a, parse_string(f, &i));
            ++nb_elts_parsed;
        }
        else if (IS_NUMBER_START(c))
        {
            StrAndLenTuple sl = parse_number(f, &i);
            if (!sl.str)
            {
                continue;
            }

            if (sl.is_float)
            {
                ro_array_add_double(a, str_to_double(&sl));
            }
            else
            {
                ro_array_add_int(a, str_to_long(&sl));
            }
            free(sl.str);
            ++nb_elts_parsed;
        }
        else if (IS_BOOL_START(c))
        {
            unsigned long len = parse_boolean(f, &i);
            if (IS_NOT_BOOLEAN(c, len))
            {
                continue;
            }
            ro_array_add_bool(a, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
        }
        else if (c == 'n')
        {
            ro_array_add_null(a);
            i += 3;
            ++nb_elts_parsed;
        }
        else if (c == '[')
        {
            unsigned long nb_chars = get_nb_chars_in_array(f, i);

            ro_array_t *tmp_ja = 0;
            // If there is enough space, we fill a buffer to read from it
            if (nb_chars < MAX_READ_BUFF_SIZE)
            {
                char *b = calloc(nb_chars + 1, sizeof(char));
                if (!b || fseek(f, i, SEEK_SET) != 0)
                {
                    free(b);
                    break;
                }
                fread(b, sizeof(char), nb_chars, f);
                tmp_ja = parse_array_buff(b, 0);
                free(b);
                i += nb_chars;
            }
            else
            {
                tmp_ja = parse_array(f, &i);
            }

            ro_array_add_array(a, tmp_ja);
            ++nb_elts_parsed;
        }
        else if (c == '{')
        {
            unsigned long nb_chars = get_nb_chars_in_dict(f, i);

            ro_dict_t *tmp_jd = 0;
            // If there is enough space, we fill a buffer to read from it
            if (nb_chars < MAX_READ_BUFF_SIZE)
            {
                char *b = calloc(nb_chars + 1, sizeof(char));
                if (!b || fseek(f, i, SEEK_SET) != 0)
                {
                    free(b);
                    break;
                }
                fread(b, sizeof(char), nb_chars, f);
                tmp_jd = parse_dict_buff(b, 0);
                free(b);
                i += nb_chars;
            }
            else
            {
                tmp_jd = parse_dict(f, &i);
            }

            ro_array_add_dict(a, tmp_jd);
            ++nb_elts_parsed;
        }
    }
    *pos = i - 1;
    return a;
}

/**
** \param f The file stream
** \param pos A pointer to the position in the file of the character after
*the
**            '{' that begins the current dict
** \returns The json dict parsed from the position
*/
ro_dict_t *parse_dict(FILE *f, unsigned long *pos)
{
    if (!f || !pos)
    {
        return 0;
    }

    unsigned long i = *pos;

    unsigned long nb_elts_parsed = 0;
    unsigned long nb_elts = get_nb_elts_dict(f, i);

    ro_dict_t *d = init_ro_dict(nb_elts);
    if (!d || nb_elts == 0)
    {
        ++(*pos);
        return d;
    }

    string_t key = EMPTY_STRING;
    char c = 0;
    char is_waiting_key = 1;
    // We start at 1 because if we entered this function, it means that we
    // already read a '['
    while (SEEK_AND_GET_CHAR(i) && nb_elts_parsed < nb_elts && c != 0)
    {
        if (c == '"')
        {
            if (is_waiting_key)
            {
                key = parse_string(f, &i);
                is_waiting_key = 0;
            }
            else
            {
                ro_dict_add_str(d, key, parse_string(f, &i));
                ++nb_elts_parsed;
            }
        }
        else if (IS_NUMBER_START(c))
        {
            StrAndLenTuple sl = parse_number(f, &i);
            if (!sl.str)
            {
                continue;
            }

            if (sl.is_float)
            {
                ro_dict_add_double(d, key, str_to_double(&sl));
            }
            else
            {
                ro_dict_add_int(d, key, str_to_long(&sl));
            }
            free(sl.str);
            ++nb_elts_parsed;
        }
        else if (IS_BOOL_START(c))
        {
            unsigned long len = parse_boolean(f, &i);
            if (IS_NOT_BOOLEAN(c, len))
            {
                continue;
            }
            ro_dict_add_bool(d, key, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
        }
        else if (c == 'n')
        {
            ro_dict_add_null(d, key);
            i += 3;
            ++nb_elts_parsed;
        }
        else if (c == '[')
        {
            unsigned long nb_chars = get_nb_chars_in_array(f, i);

            ro_array_t *tmp_ja = 0;
            // If there is enough space, we fill a buffer to read from it
            if (nb_chars < MAX_READ_BUFF_SIZE)
            {
                char *b = calloc(nb_chars + 1, sizeof(char));
                if (!b || fseek(f, i, SEEK_SET) != 0)
                {
                    free(b);
                    break;
                }
                fread(b, sizeof(char), nb_chars, f);
                tmp_ja = parse_array_buff(b, 0);
                free(b);
                i += nb_chars;
            }
            else
            {
                tmp_ja = parse_array(f, &i);
            }

            ro_dict_add_array(d, key, tmp_ja);
            ++nb_elts_parsed;
        }
        else if (c == '{')
        {
            unsigned long nb_chars = get_nb_chars_in_dict(f, i);

            ro_dict_t *tmp_jd = 0;
            // If there is enough space, we fill a buffer to read from it
            if (nb_chars < MAX_READ_BUFF_SIZE)
            {
                char *b = calloc(nb_chars + 1, sizeof(char));
                if (!b || fseek(f, i, SEEK_SET) != 0)
                {
                    free(b);
                    break;
                }
                fread(b, sizeof(char), nb_chars, f);
                tmp_jd = parse_dict_buff(b, 0);
                free(b);
                i += nb_chars;
            }
            else
            {
                tmp_jd = parse_dict(f, &i);
            }

            ro_dict_add_dict(d, key, tmp_jd);
            ++nb_elts_parsed;
        }
        else if (c == ',')
        {
            is_waiting_key = 1;
        }
    }
    *pos = i - 1;
    return d;
}

/*******************************************************************************
**                                 FUNCTIONS **
*******************************************************************************/
ro_json_t *ro_parse(char *file)
{
    FILE *f = fopen(file, "r");
    if (!f)
    {
        return 0;
    }

    unsigned long offset = 0;
    if (fseek(f, offset++, SEEK_SET) != 0)
    {
        fclose(f);
        return 0;
    }

    // Obtains the number of characters in the file
    struct stat st;
    stat(file, &st);
    unsigned long nb_chars = st.st_size;

    char c = fgetc(f);
    if (c == '{')
    {
        ro_dict_t *d = 0;
        if (nb_chars < MAX_READ_BUFF_SIZE)
        {
            char *b = calloc(nb_chars + 1, sizeof(char));
            if (!b || fseek(f, offset, SEEK_SET) != 0)
            {
                fclose(f);
                free(b);
                return 0;
            }
            fread(b, sizeof(char), nb_chars, f);
            d = parse_dict_buff(b, 0);
            free(b);
        }
        else
        {
            d = parse_dict(f, &offset);
        }
        fclose(f);
        return init_ro_json(0, 0, d);
    }
    else if (c == '[')
    {
        ro_array_t *a = 0;
        if (nb_chars < MAX_READ_BUFF_SIZE)
        {
            char *b = calloc(nb_chars + 1, sizeof(char));
            if (!b || fseek(f, offset, SEEK_SET) != 0)
            {
                fclose(f);
                free(b);
                return 0;
            }
            fread(b, sizeof(char), nb_chars, f);
            a = parse_array_buff(b, 0);
            free(b);
        }
        else
        {
            a = parse_array(f, &offset);
        }
        fclose(f);
        return init_ro_json(1, a, 0);
    }
    fclose(f);
    return 0;
}
