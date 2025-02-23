#include "base_json_parser.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "base_json_storage.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
long_with_or_without_exponent_t str_to_long(str_and_len_tuple_t *sl)
{
    if (!sl)
    {
        return ERROR_LWOWE;
    }

    char *str = sl->str;
    unsigned long len = sl->len;
    if (!str || len == 0)
    {
        return ERROR_LWOWE;
    }

    char has_exponent = sl->has_exponent;

    long number = 0;
    long exponent = 0;
    unsigned char exp_idx = 0;
    char is_negative = str[0] == '-' ? -1 : 1;
    char is_exp_negative = 1;
    char is_in_exponent = 0;
    char c = 0;
    for (unsigned long i = 0; i < len; ++i)
    {
        c = str[i];
        if (has_exponent && (c == 'e' || c == 'E'))
        {
            exp_idx = i;
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
                number = number * 10 + c - '0';
            }
        }
        else if (c == '-' && exp_idx && i - 1 == exp_idx)
        {
            is_exp_negative = -1;
        }
    }
    if (has_exponent)
    {
        return (long_with_or_without_exponent_t){
            .has_exponent = 1,
            .long_exp_value =
                (exponent_long_t){ .number = number * is_negative,
                                   .exponent = exponent * is_exp_negative },
            .long_value = 0
        };
    }
    return (long_with_or_without_exponent_t){
        .has_exponent = 0,
        .long_value = number * is_negative,
        .long_exp_value = (exponent_long_t){ .number = 0, .exponent = 0 }
    };
}

double_with_or_without_exponent_t str_to_double(str_and_len_tuple_t *sl)
{
    if (!sl)
    {
        return ERROR_DWOWE;
    }

    char *str = sl->str;
    unsigned long len = sl->len;
    if (!str || len == 0)
    {
        return ERROR_DWOWE;
    }

    char has_exponent = sl->has_exponent;

    double number = 0; // Integer part
    double decimals = 0; // Decimal part
    long exponent = 0; // Only used if sl->has_exponent is true
    unsigned long nb_digits_decimals = 1;
    unsigned char exp_idx = 0;
    // If the number is negative, this is set to -1 and the final res is
    // multiplied by it
    char is_negative = str[0] == '-' ? -1 : 1;
    char is_exp_negative = 1;
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
        else if (has_exponent && (c == 'e' || c == 'E'))
        {
            exp_idx = i;
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
                decimals = decimals * 10 + c - '0';
                nb_digits_decimals *= 10;
            }
            else
            {
                number = number * 10 + c - '0';
            }
        }
        else if (c == '-' && exp_idx && i - 1 == exp_idx)
        {
            is_exp_negative = -1;
        }
    }
    if (has_exponent)
    {
        return (double_with_or_without_exponent_t){
            .has_exponent = 1,
            .double_exp_value =
                (exponent_double_t){
                    .number = (number + (decimals / nb_digits_decimals))
                        * is_negative,
                    .exponent = exponent * is_exp_negative },
            .double_value = 0
        };
    }
    return (double_with_or_without_exponent_t){
        .has_exponent = 0,
        .double_value =
            (number + (decimals / nb_digits_decimals)) * is_negative,
        .double_exp_value = (exponent_double_t){ .number = 0, .exponent = 0 }
    };
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

/***************************************
**              PARSING               **
***************************************/
string_t parse_string_buff(char *buff, unsigned long *idx)
{
    if (!buff || !idx)
    {
        return NULL_STRING;
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

    if (!len)
    {
        ++(*idx);
        return STRING_NOFREE_OF("", 0);
    }

    char *str = calloc(len + 1, sizeof(char));
    if (!str)
    {
        return NULL_STRING;
    }
    memcpy(str, buff + start_idx, len);

    // + 1 to not read the last '"' when returning in the calling function
    *idx += len + 1;
    return STRING_OF(str, len);
}

string_t parse_string(FILE *f, unsigned long *pos)
{
    if (!f || !pos)
    {
        return NULL_STRING;
    }

    unsigned long i = *pos;
    char c = 0;
    char prev_c = 0;
    while (SEEK_AND_GET_CHAR(i) && !IS_STRING_END(c))
    {
        prev_c = c;
    }

    unsigned len = i - *pos - 1;
    if (!len)
    {
        ++(*pos);
        return STRING_NOFREE_OF("", 0);
    }

    char *str = calloc(len + 1, sizeof(char));
    if (!str)
    {
        return NULL_STRING;
    }

    if (fseek(f, *pos, SEEK_SET) != 0)
    {
        free(str);
        return NULL_STRING;
    }
    fread(str, sizeof(char), len, f);

    // +1 to not read the last '"' when returning in the calling function
    *pos += len + 1;
    return STRING_OF(str, len);
}

str_and_len_tuple_t parse_number_buff(char *buff, unsigned long *idx)
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
    return STR_AND_LEN_OF(str, len, is_float(str, len), has_exponent(str, len));
}

str_and_len_tuple_t parse_number(FILE *f, unsigned long *pos)
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
    return STR_AND_LEN_OF(str, len, is_float(str, len), has_exponent(str, len));
}

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

        if (!is_in_array)
        {
            break;
        }
        ++nb_chars;
    }
    return nb_chars;
}

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

        if (!is_in_dict)
        {
            break;
        }
        ++nb_chars;
    }
    return nb_chars;
}
