#include "base_json_parser.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
extern inline size_t get_str_len(char *buff, size_t pos);

size_t get_num_len(char *buff, size_t pos)
{
    if (!buff)
    {
        return 0;
    }

    size_t nb_chars = 0;

    char c = 0;
    while ((c = buff[pos++]))
    {
        switch (c)
        {
        case '+':
        case '-':
        case '.':
        case 'e':
        case 'E':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            ++nb_chars;
            break;
        default:
            return nb_chars;
        }
    }
    return 0;
}

long_with_or_without_exponent_t str_to_long(str_and_len_tuple_t *sl)
{
    if (!sl)
    {
        return ERROR_LWOWE;
    }

    char *str  = sl->str;
    size_t len = sl->len;
    if (!str || len == 0)
    {
        return ERROR_LWOWE;
    }

    size_t exp_idx = 0;

    i64 number   = 0;
    i64 exponent = 0;

    bool is_in_exponent = false;

    u8 has_exponent    = sl->has_exponent;
    i8 is_negative     = str[0] == '-' ? -1 : 1;
    i8 is_exp_negative = 1;

    char c = 0;
    for (size_t i = 0; i < len; ++i)
    {
        switch ((c = str[i]))
        {
        case 'e':
        case 'E':
            if (has_exponent)
            {
                exp_idx        = i;
                is_in_exponent = true;
            }
            break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if (is_in_exponent)
            {
                exponent = exponent * 10 + c - '0';
            }
            else
            {
                number = number * 10 + c - '0';
            }
            break;

        case '-':
            if (exp_idx && i - 1 == exp_idx)
            {
                is_exp_negative = -1;
            }
            break;
        }
    }
    if (has_exponent)
    {
        return (long_with_or_without_exponent_t
        ){ .has_exponent = 1,
           .long_exp_value =
               (exp_long_t){ .number   = number * is_negative,
                             .exponent = exponent * is_exp_negative },
           .long_value = 0 };
    }
    return (long_with_or_without_exponent_t
    ){ .has_exponent   = 0,
       .long_value     = number * is_negative,
       .long_exp_value = (exp_long_t){ .number = 0, .exponent = 0 } };
}

double_with_or_without_exponent_t str_to_double(str_and_len_tuple_t *sl)
{
    if (!sl)
    {
        return ERROR_DWOWE;
    }

    char *str  = sl->str;
    size_t len = sl->len;
    if (!str || len == 0)
    {
        return ERROR_DWOWE;
    }

    double number   = 0; // Integer part
    double decimals = 0; // Decimal part
    i64 exponent    = 0; // Only used if sl->has_exponent is true

    i64 nb_digits_decimals = 1;
    size_t exp_idx         = 0;

    bool dot_reached    = false;
    bool is_in_exponent = false;
    u8 has_exponent     = sl->has_exponent;

    // If the number is negative, this is set to -1 and the final res is
    // multiplied by it
    i8 is_negative     = str[0] == '-' ? -1 : 1;
    i8 is_exp_negative = 1;

    char c = 0;
    for (size_t i = 0; i < len; ++i)
    {
        switch (c = str[i])
        {
        case '.':
            dot_reached = true;
            break;

        case 'e':
        case 'E':
            if (has_exponent)
            {
                exp_idx        = i;
                is_in_exponent = true;
            }
            break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
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
            break;

        case '-':
            if (exp_idx && i - 1 == exp_idx)
            {
                is_exp_negative = -1;
            }
            break;
        }
    }
    if (has_exponent)
    {
        return (double_with_or_without_exponent_t
        ){ .has_exponent     = 1,
           .double_exp_value = (exp_double_t
           ){ .number =
                  (number + (decimals / nb_digits_decimals)) * is_negative,
              .exponent = exponent * is_exp_negative },
           .double_value     = 0 };
    }
    return (double_with_or_without_exponent_t
    ){ .has_exponent = 0,
       .double_value = (number + (decimals / nb_digits_decimals)) * is_negative,
       .double_exp_value = (exp_double_t){ .number = 0, .exponent = 0 } };
}

bool is_float(char *str, size_t len)
{
    if (!str)
    {
        return false;
    }

    switch (len)
    {
    case 1:
    case 2:
        return false;

    case 3:
        return str[1] == '.';
    case 4:
        return str[1] == '.' || str[2] == '.';
    case 5:
        return str[1] == '.' || str[2] == '.' || str[3] == '.';
    case 6:
        return str[1] == '.' || str[2] == '.' || str[3] == '.' || str[4] == '.';
    case 7:
        return str[1] == '.' || str[2] == '.' || str[3] == '.' || str[4] == '.'
            || str[5] == '.';
    case 8:
        return str[1] == '.' || str[2] == '.' || str[3] == '.' || str[4] == '.'
            || str[5] == '.' || str[6] == '.';

    default:
        for (size_t i = 1; i < len - 1; ++i)
        {
            if (str[i] == '.')
            {
                return true;
            }
        }
        break;
    }
    return false;
}

bool has_exponent(char *str, size_t len)
{
    if (!str)
    {
        return false;
    }

    switch (len)
    {
    case 1:
    case 2:
        return false;

    case 3:
        return str[1] == 'e' || str[1] == 'E';
    case 4:
        return str[1] == 'e' || str[2] == 'e' || str[1] == 'E' || str[2] == 'E';
    case 5:
        return str[1] == 'e' || str[1] == 'E' || str[2] == 'e' || str[2] == 'E'
            || str[3] == 'e' || str[3] == 'E';
    case 6:
        return str[1] == 'e' || str[1] == 'E' || str[2] == 'e' || str[2] == 'E'
            || str[3] == 'e' || str[3] == 'E' || str[4] == 'e' || str[4] == 'E';
    case 7:
        return str[1] == 'e' || str[1] == 'E' || str[2] == 'e' || str[2] == 'E'
            || str[3] == 'e' || str[3] == 'E' || str[4] == 'e' || str[4] == 'E'
            || str[5] == 'e' || str[5] == 'E';
    case 8:
        return str[1] == 'e' || str[1] == 'E' || str[2] == 'e' || str[2] == 'E'
            || str[3] == 'e' || str[3] == 'E' || str[4] == 'e' || str[4] == 'E'
            || str[5] == 'e' || str[5] == 'E' || str[6] == 'e' || str[6] == 'E';

    default:
        for (size_t i = 1; i < len - 1; ++i)
        {
            switch (str[i])
            {
            case 'e':
            case 'E':
                return true;
            }
        }
        break;
    }
    return false;
}

/***************************************
**              PARSING               **
***************************************/
string_t parse_string(char *buff, size_t *idx)
{
    if (!buff || !idx)
    {
        return NULL_STRING;
    }

    size_t start_idx = *idx + 1;
    size_t len       = 0;
    char c           = 0;
    char prev_c      = 0;
    // Counts the number of characters
    while ((c = buff[start_idx + len]))
    {
        if (c == '"' && prev_c != '\\')
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

    char *str = malloc((len + 1) * sizeof(char));
    if (!str)
    {
        return NULL_STRING;
    }
    memcpy(str, buff + start_idx, len);
    str[len] = 0;

    // + 1 to not read the last '"' when returning in the calling function
    *idx += len + 1;
    return STRING_OF(str, len);
}

str_and_len_tuple_t parse_number(char *buff, size_t *idx)
{
    if (!buff || !idx)
    {
        return NULL_STR_AND_LEN_TUPLE;
    }

    // Counts the number of characters until the first one that is an 'end char'
    size_t end_idx = *idx;
    size_t size    = 0;
    char c         = 0;
    while ((c = buff[end_idx++]))
    {
        switch (c)
        {
        case '+':
        case '-':
        case 'e':
        case 'E':
        case '.':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            break;

        default:
            // Number of chars
            if (!size)
            {
                return NULL_STR_AND_LEN_TUPLE;
            }

            // Puts the value in the form of a char array
            char *str = malloc((size + 1) * sizeof(char));
            if (!str)
            {
                return NULL_STR_AND_LEN_TUPLE;
            }
            memcpy(str, buff + *idx, size);
            str[size] = 0;

            *idx += size - 1;
            return STR_AND_LEN_OF(
                str, size, is_float(str, size), has_exponent(str, size)
            );
        }
        ++size;
    }
    return NULL_STR_AND_LEN_TUPLE;
}

size_t parse_boolean(char *buff, size_t *idx)
{
    if (!buff || !idx)
    {
        return 0;
    }

    size_t end_idx = *idx;
    size_t size    = 0;
    char c         = 0;
    while ((c = buff[end_idx++]))
    {
        switch (c)
        {
        case 't':
        case 'r':
        case 'u':
        case 'e':
        case 'f':
        case 'a':
        case 'l':
        case 's':
            break;

        default:
            *idx += size - 1;
            return size;
        }
        ++size;
    }
    return 0;
}

size_t get_nb_elts_array(char *buff, size_t idx)
{
    if (!buff || buff[idx] == ']')
    {
        return 0;
    }

    size_t nb_elts = 0;

    // Counts the number of nested arrays/dicts
    u64 array_count = 1;
    u64 dict_count  = 0;

    char c = 0;
    while ((c = buff[idx++]) && array_count)
    {
        switch (c)
        {
        case '"':
        case '+':
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 't':
        case 'f':
        case 'n':
        case '[':
        case '{':
            if (!dict_count && array_count == 1)
            {
                ++nb_elts;
            }
        }

        switch (c)
        {
        case '"':
            idx += get_str_len(buff, idx);
            break;

        case 't':
            idx += 3;
            break;

        case 'f':
            idx += 4;
            break;

        case 'n':
            idx += 3;
            break;

        case '+':
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            idx += get_num_len(buff, idx);
            break;

        case '[':
            ++array_count;
            break;

        case ']':
            --array_count;
            break;

        case '{':
            ++dict_count;
            break;

        case '}':
            --dict_count;
            break;
        }
    }
    return nb_elts;
}

size_t get_nb_elts_dict(char *buff, size_t idx)
{
    if (!buff || buff[idx] == '}')
    {
        return 0;
    }

    size_t nb_elts = 0;

    // Counts the number of arrays/dicts nesting at the current position
    u64 dict_count  = 1;
    u64 array_count = 0;

    bool is_in_key = true;

    char c = 0;
    while ((c = buff[idx++]) && dict_count)
    {
        if (!is_in_key)
        {
            switch (c)
            {
            case '"':
            case '+':
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 't':
            case 'f':
            case 'n':
            case '[':
            case '{':
                if (!array_count && dict_count == 1)
                {
                    ++nb_elts;
                }
            }
        }

        switch (c)
        {
        case ':':
            is_in_key = false;
            break;

        case ',':
            is_in_key = true;
            break;

        case '"':
            idx += get_str_len(buff, idx);
            break;

        case 't':
            idx += 3;
            break;

        case 'f':
            idx += 4;
            break;

        case 'n':
            idx += 3;
            break;

        case '+':
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            idx += get_num_len(buff, idx);
            break;

        case '[':
            ++array_count;
            break;

        case ']':
            --array_count;
            break;

        case '{':
            ++dict_count;
            break;

        case '}':
            --dict_count;
            break;
        }
    }
    return nb_elts;
}
