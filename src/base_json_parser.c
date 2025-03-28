#include "base_json_parser.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
bool is_float(const char *str, size_t len)
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

bool has_exponent(const char *s, size_t len)
{
    if (!s)
    {
        return false;
    }

    switch (len)
    {
    case 1:
    case 2:
        return false;

    case 3:
        return s[1] == 'e' || s[1] == 'E';
    case 4:
        return s[1] == 'e' || s[2] == 'e' || s[1] == 'E' || s[2] == 'E';
    case 5:
        return s[1] == 'e' || s[1] == 'E' || s[2] == 'e' || s[2] == 'E'
            || s[3] == 'e' || s[3] == 'E';
    case 6:
        return s[1] == 'e' || s[1] == 'E' || s[2] == 'e' || s[2] == 'E'
            || s[3] == 'e' || s[3] == 'E' || s[4] == 'e' || s[4] == 'E';
    case 7:
        return s[1] == 'e' || s[1] == 'E' || s[2] == 'e' || s[2] == 'E'
            || s[3] == 'e' || s[3] == 'E' || s[4] == 'e' || s[4] == 'E'
            || s[5] == 'e' || s[5] == 'E';
    case 8:
        return s[1] == 'e' || s[1] == 'E' || s[2] == 'e' || s[2] == 'E'
            || s[3] == 'e' || s[3] == 'E' || s[4] == 'e' || s[4] == 'E'
            || s[5] == 'e' || s[5] == 'E' || s[6] == 'e' || s[6] == 'E';
    case 9:
        return s[1] == 'e' || s[1] == 'E' || s[2] == 'e' || s[2] == 'E'
            || s[3] == 'e' || s[3] == 'E' || s[4] == 'e' || s[4] == 'E'
            || s[5] == 'e' || s[5] == 'E' || s[6] == 'e' || s[6] == 'E'
            || s[7] == 'e' || s[8] == 'E';
    case 10:
        return s[1] == 'e' || s[1] == 'E' || s[2] == 'e' || s[2] == 'E'
            || s[3] == 'e' || s[3] == 'E' || s[4] == 'e' || s[4] == 'E'
            || s[5] == 'e' || s[5] == 'E' || s[6] == 'e' || s[6] == 'E'
            || s[7] == 'e' || s[7] == 'E' || s[8] == 'e' || s[8] == 'E';
    case 11:
        return s[1] == 'e' || s[1] == 'E' || s[2] == 'e' || s[2] == 'E'
            || s[3] == 'e' || s[3] == 'E' || s[4] == 'e' || s[4] == 'E'
            || s[5] == 'e' || s[5] == 'E' || s[6] == 'e' || s[6] == 'E'
            || s[7] == 'e' || s[7] == 'E' || s[8] == 'e' || s[8] == 'E'
            || s[9] == 'e' || s[9] == 'E';
    case 12:
        return s[1] == 'e' || s[1] == 'E' || s[2] == 'e' || s[2] == 'E'
            || s[3] == 'e' || s[3] == 'E' || s[4] == 'e' || s[4] == 'E'
            || s[5] == 'e' || s[5] == 'E' || s[6] == 'e' || s[6] == 'E'
            || s[7] == 'e' || s[7] == 'E' || s[8] == 'e' || s[8] == 'E'
            || s[9] == 'e' || s[9] == 'E' || s[10] == 'e' || s[10] == 'E';
    case 13:
        return s[1] == 'e' || s[1] == 'E' || s[2] == 'e' || s[2] == 'E'
            || s[3] == 'e' || s[3] == 'E' || s[4] == 'e' || s[4] == 'E'
            || s[5] == 'e' || s[5] == 'E' || s[6] == 'e' || s[6] == 'E'
            || s[7] == 'e' || s[7] == 'E' || s[8] == 'e' || s[8] == 'E'
            || s[9] == 'e' || s[9] == 'E' || s[10] == 'e' || s[10] == 'E'
            || s[11] == 'e' || s[11] == 'E';
    case 14:
        return s[1] == 'e' || s[1] == 'E' || s[2] == 'e' || s[2] == 'E'
            || s[3] == 'e' || s[3] == 'E' || s[4] == 'e' || s[4] == 'E'
            || s[5] == 'e' || s[5] == 'E' || s[6] == 'e' || s[6] == 'E'
            || s[7] == 'e' || s[7] == 'E' || s[8] == 'e' || s[8] == 'E'
            || s[9] == 'e' || s[9] == 'E' || s[10] == 'e' || s[10] == 'E'
            || s[11] == 'e' || s[11] == 'E' || s[12] == 'e' || s[12] == 'E';
    case 15:
        return s[1] == 'e' || s[1] == 'E' || s[2] == 'e' || s[2] == 'E'
            || s[3] == 'e' || s[3] == 'E' || s[4] == 'e' || s[4] == 'E'
            || s[5] == 'e' || s[5] == 'E' || s[6] == 'e' || s[6] == 'E'
            || s[7] == 'e' || s[7] == 'E' || s[8] == 'e' || s[8] == 'E'
            || s[9] == 'e' || s[9] == 'E' || s[10] == 'e' || s[10] == 'E'
            || s[11] == 'e' || s[11] == 'E' || s[12] == 'e' || s[12] == 'E'
            || s[13] == 'e' || s[13] == 'E';
    case 16:
        return s[1] == 'e' || s[1] == 'E' || s[2] == 'e' || s[2] == 'E'
            || s[3] == 'e' || s[3] == 'E' || s[4] == 'e' || s[4] == 'E'
            || s[5] == 'e' || s[5] == 'E' || s[6] == 'e' || s[6] == 'E'
            || s[7] == 'e' || s[7] == 'E' || s[8] == 'e' || s[8] == 'E'
            || s[9] == 'e' || s[9] == 'E' || s[10] == 'e' || s[10] == 'E'
            || s[11] == 'e' || s[11] == 'E' || s[12] == 'e' || s[12] == 'E'
            || s[13] == 'e' || s[13] == 'E' || s[14] == 'e' || s[14] == 'E';
    case 17:
        return s[1] == 'e' || s[1] == 'E' || s[2] == 'e' || s[2] == 'E'
            || s[3] == 'e' || s[3] == 'E' || s[4] == 'e' || s[4] == 'E'
            || s[5] == 'e' || s[5] == 'E' || s[6] == 'e' || s[6] == 'E'
            || s[7] == 'e' || s[7] == 'E' || s[8] == 'e' || s[8] == 'E'
            || s[9] == 'e' || s[9] == 'E' || s[10] == 'e' || s[10] == 'E'
            || s[11] == 'e' || s[11] == 'E' || s[12] == 'e' || s[12] == 'E'
            || s[13] == 'e' || s[13] == 'E' || s[14] == 'e' || s[14] == 'E'
            || s[15] == 'e' || s[15] == 'E';
    case 18:
        return s[1] == 'e' || s[1] == 'E' || s[2] == 'e' || s[2] == 'E'
            || s[3] == 'e' || s[3] == 'E' || s[4] == 'e' || s[4] == 'E'
            || s[5] == 'e' || s[5] == 'E' || s[6] == 'e' || s[6] == 'E'
            || s[7] == 'e' || s[7] == 'E' || s[8] == 'e' || s[8] == 'E'
            || s[9] == 'e' || s[9] == 'E' || s[10] == 'e' || s[10] == 'E'
            || s[11] == 'e' || s[11] == 'E' || s[12] == 'e' || s[12] == 'E'
            || s[13] == 'e' || s[13] == 'E' || s[14] == 'e' || s[14] == 'E'
            || s[15] == 'e' || s[15] == 'E' || s[16] == 'e' || s[16] == 'E';
    case 19:
        return s[1] == 'e' || s[1] == 'E' || s[2] == 'e' || s[2] == 'E'
            || s[3] == 'e' || s[3] == 'E' || s[4] == 'e' || s[4] == 'E'
            || s[5] == 'e' || s[5] == 'E' || s[6] == 'e' || s[6] == 'E'
            || s[7] == 'e' || s[7] == 'E' || s[8] == 'e' || s[8] == 'E'
            || s[9] == 'e' || s[9] == 'E' || s[10] == 'e' || s[10] == 'E'
            || s[11] == 'e' || s[11] == 'E' || s[12] == 'e' || s[12] == 'E'
            || s[13] == 'e' || s[13] == 'E' || s[14] == 'e' || s[14] == 'E'
            || s[15] == 'e' || s[15] == 'E' || s[16] == 'e' || s[16] == 'E'
            || s[17] == 'e' || s[17] == 'E';
    case 20:
        return s[1] == 'e' || s[1] == 'E' || s[2] == 'e' || s[2] == 'E'
            || s[3] == 'e' || s[3] == 'E' || s[4] == 'e' || s[4] == 'E'
            || s[5] == 'e' || s[5] == 'E' || s[6] == 'e' || s[6] == 'E'
            || s[7] == 'e' || s[7] == 'E' || s[8] == 'e' || s[8] == 'E'
            || s[9] == 'e' || s[9] == 'E' || s[10] == 'e' || s[10] == 'E'
            || s[11] == 'e' || s[11] == 'E' || s[12] == 'e' || s[12] == 'E'
            || s[13] == 'e' || s[13] == 'E' || s[14] == 'e' || s[14] == 'E'
            || s[15] == 'e' || s[15] == 'E' || s[16] == 'e' || s[16] == 'E'
            || s[17] == 'e' || s[17] == 'E' || s[18] == 'e' || s[18] == 'E';

    default:
        for (size_t i = 1; i < len - 1; ++i)
        {
            switch (s[i])
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

    double nb_digits_decimals = 1;

    double number   = 0; // Integer part
    double decimals = 0; // Decimal part
    i64 exponent    = 0; // Only used if sl->has_exponent is true

    size_t exp_idx = 0;

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
    // Counts the number of characters
    while ((c = buff[start_idx + len]))
    {
        if (c == '"' && start_idx + len > 1
            && buff[start_idx + len - 2] != '\\')
        {
            break;
        }
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
    size_t end_idx  = *idx;
    size_t nb_chars = 0;
    char c          = 0;
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
            ++nb_chars;
            continue;
        }
        break;
    }

    if (!nb_chars)
    {
        return NULL_STR_AND_LEN_TUPLE;
    }

    // Puts the value in the form of a char array
    char *str = malloc((nb_chars + 1) * sizeof(char));
    if (!str)
    {
        return NULL_STR_AND_LEN_TUPLE;
    }
    memcpy(str, buff + *idx, nb_chars);
    str[nb_chars] = 0;

    *idx += nb_chars - 1;
    return STR_AND_LEN_OF(
        str, nb_chars, is_float(str, nb_chars), has_exponent(str, nb_chars)
    );
}

size_t parse_boolean(const char *buff, size_t *idx)
{
    if (!buff || !idx)
    {
        return 0;
    }

    size_t end_idx  = *idx;
    size_t nb_chars = 0;
    char c          = 0;
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
            ++nb_chars;
            continue;
        }
        break;
    }
    *idx += nb_chars - 1;
    return nb_chars;
}

#define GET_NB_ELTS_ARRAY_INC                                                  \
    if (!dict_count && array_count == 1)                                       \
    {                                                                          \
        ++nb_elts;                                                             \
    }

size_t get_nb_elts_array(const char *buff, size_t idx)
{
    if (!buff)
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
            GET_NB_ELTS_ARRAY_INC;
            GET_STR_LEN(idx);
            break;

        case 't':
        case 'n':
            GET_NB_ELTS_ARRAY_INC;
            idx += 3;
            break;

        case 'f':
            GET_NB_ELTS_ARRAY_INC;
            idx += 4;
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
            GET_NB_ELTS_ARRAY_INC;
            GET_NUM_LEN(idx);
            break;

        case '[':
            GET_NB_ELTS_ARRAY_INC;
            ++array_count;
            break;

        case '{':
            GET_NB_ELTS_ARRAY_INC;
            ++dict_count;
            break;

        case ']':
            --array_count;
            break;

        case '}':
            --dict_count;
            break;
        }
    }
    return nb_elts;
}

#define GET_NB_ELTS_DICT_INC                                                   \
    if (!is_in_key && !array_count && dict_count == 1)                         \
    {                                                                          \
        ++nb_elts;                                                             \
    }

size_t get_nb_elts_dict(const char *buff, size_t idx)
{
    if (!buff)
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
        switch (c)
        {
        case ':':
            is_in_key = false;
            break;

        case ',':
            is_in_key = true;
            break;

        case '"':
            GET_NB_ELTS_DICT_INC;
            GET_STR_LEN(idx);
            break;

        case 't':
        case 'n':
            GET_NB_ELTS_DICT_INC;
            idx += 3;
            break;

        case 'f':
            GET_NB_ELTS_DICT_INC;
            idx += 4;
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
            GET_NB_ELTS_DICT_INC;
            GET_NUM_LEN(idx);
            break;

        case '[':
            GET_NB_ELTS_DICT_INC;
            ++array_count;
            break;

        case '{':
            GET_NB_ELTS_DICT_INC;
            ++dict_count;
            break;

        case ']':
            --array_count;
            break;

        case '}':
            --dict_count;
            break;
        }
    }
    return nb_elts;
}
