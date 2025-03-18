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
#define CHECK_STR_LEN                                                          \
    ++nb_chars;                                                                \
    if (c == '"' && prev_c != '\\')                                            \
    {                                                                          \
        break;                                                                 \
    }                                                                          \
    prev_c = c;

/**
** \returns The number of character in the string + 1 (for the last quote '"')
*/
size_t get_str_len_buff(char *buff, size_t pos)
{
    if (!buff)
    {
        return 1;
    }

    size_t nb_chars = 0;

    char c = 0;
    char prev_c = 0;
    while ((c = buff[pos++]))
    {
        CHECK_STR_LEN
    }
    return nb_chars;
}

size_t get_str_len_file(FILE *f, size_t pos)
{
    if (!f)
    {
        return 1;
    }

    size_t nb_chars = 0;

    char c = 0;
    char prev_c = 0;
    while (SEEK_AND_GET_CHAR(pos))
    {
        CHECK_STR_LEN
    }
    return nb_chars;
}

#define CHECK_NUM_LEN                                                          \
    switch (c)                                                                 \
    {                                                                          \
    case '+':                                                                  \
    case '-':                                                                  \
    case '.':                                                                  \
    case 'e':                                                                  \
    case 'E':                                                                  \
    case '0':                                                                  \
    case '1':                                                                  \
    case '2':                                                                  \
    case '3':                                                                  \
    case '4':                                                                  \
    case '5':                                                                  \
    case '6':                                                                  \
    case '7':                                                                  \
    case '8':                                                                  \
    case '9':                                                                  \
        ++nb_chars;                                                            \
        break;                                                                 \
    default:                                                                   \
        return nb_chars;                                                       \
    }

size_t get_num_len_buff(char *buff, size_t pos)
{
    if (!buff)
    {
        return 0;
    }

    size_t nb_chars = 0;

    char c = 0;
    while ((c = buff[pos++]))
    {
        CHECK_NUM_LEN
    }
    return 0;
}

size_t get_num_len_file(FILE *f, size_t pos)
{
    if (!f)
    {
        return 0;
    }

    size_t nb_chars = 0;

    char c = 0;
    while (SEEK_AND_GET_CHAR(pos))
    {
        CHECK_NUM_LEN
    }
    return 0;
}

long_with_or_without_exponent_t str_to_long(str_and_len_tuple_t *sl)
{
    if (!sl)
    {
        return ERROR_LWOWE;
    }

    char *str = sl->str;
    size_t len = sl->len;
    if (!str || len == 0)
    {
        return ERROR_LWOWE;
    }

    size_t exp_idx = 0;

    i64 number = 0;
    i64 exponent = 0;

    bool is_in_exponent = false;

    u8 has_exponent = sl->has_exponent;
    i8 is_negative = str[0] == '-' ? -1 : 1;
    i8 is_exp_negative = 1;

    char c = 0;
    for (size_t i = 0; i < len; ++i)
    {
        c = str[i];
        if (has_exponent && (c == 'e' || c == 'E'))
        {
            exp_idx = i;
            is_in_exponent = true;
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
                (exp_long_t){ .number = number * is_negative,
                              .exponent = exponent * is_exp_negative },
            .long_value = 0
        };
    }
    return (long_with_or_without_exponent_t){
        .has_exponent = 0,
        .long_value = number * is_negative,
        .long_exp_value = (exp_long_t){ .number = 0, .exponent = 0 }
    };
}

double_with_or_without_exponent_t str_to_double(str_and_len_tuple_t *sl)
{
    if (!sl)
    {
        return ERROR_DWOWE;
    }

    char *str = sl->str;
    size_t len = sl->len;
    if (!str || len == 0)
    {
        return ERROR_DWOWE;
    }

    double number = 0; // Integer part
    double decimals = 0; // Decimal part
    i64 exponent = 0; // Only used if sl->has_exponent is true

    i64 nb_digits_decimals = 1;
    size_t exp_idx = 0;

    bool dot_reached = false;
    bool is_in_exponent = false;
    u8 has_exponent = sl->has_exponent;

    // If the number is negative, this is set to -1 and the final res is
    // multiplied by it
    i8 is_negative = str[0] == '-' ? -1 : 1;
    i8 is_exp_negative = 1;

    char c = 0;
    for (size_t i = 0; i < len; ++i)
    {
        c = str[i];
        if (c == '.')
        {
            dot_reached = true;
        }
        else if (has_exponent && (c == 'e' || c == 'E'))
        {
            exp_idx = i;
            is_in_exponent = true;
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
                (exp_double_t){ .number =
                                    (number + (decimals / nb_digits_decimals))
                                    * is_negative,
                                .exponent = exponent * is_exp_negative },
            .double_value = 0
        };
    }
    return (double_with_or_without_exponent_t){
        .has_exponent = 0,
        .double_value =
            (number + (decimals / nb_digits_decimals)) * is_negative,
        .double_exp_value = (exp_double_t){ .number = 0, .exponent = 0 }
    };
}

bool is_float(char *str, size_t len)
{
    if (!str)
    {
        return false;
    }

    for (size_t i = 0; i < len; ++i)
    {
        if (str[i] == '.')
        {
            return true;
        }
    }
    return false;
}

bool has_exponent(char *str, size_t len)
{
    if (!str)
    {
        return false;
    }

    for (size_t i = 0; i < len; ++i)
    {
        switch (str[i])
        {
        case 'e':
        case 'E':
            return true;
        }
    }
    return false;
}

/***************************************
**              PARSING               **
***************************************/
string_t parse_string_buff(char *buff, size_t *idx)
{
    if (!buff || !idx)
    {
        return NULL_STRING;
    }

    size_t start_idx = *idx + 1;
    size_t len = 0;
    char c = 0;
    char prev_c = 0;
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

string_t parse_string(FILE *f, size_t *pos)
{
    if (!f || !pos)
    {
        return NULL_STRING;
    }

    size_t i = *pos;
    char c = 0;
    char prev_c = 0;
    while (SEEK_AND_GET_CHAR(i) && !IS_STRING_END(c))
    {
        prev_c = c;
    }

    size_t len = i - *pos - 1;
    if (!len)
    {
        ++(*pos);
        return STRING_NOFREE_OF("", 0);
    }

    char *str = malloc((len + 1) * sizeof(char));
    if (!str)
    {
        return NULL_STRING;
    }
    str[len] = 0;

    if (fseek(f, *pos, SEEK_SET))
    {
        free(str);
        return NULL_STRING;
    }
    fread(str, sizeof(char), len, f);

    // +1 to not read the last '"' when returning in the calling function
    *pos += len + 1;
    return STRING_OF(str, len);
}

str_and_len_tuple_t parse_number_buff(char *buff, size_t *idx)
{
    if (!buff || !idx)
    {
        return NULL_STR_AND_LEN_TUPLE;
    }

    // Counts the number of characters until the first one that is an 'end char'
    size_t end_idx = *idx;
    size_t size = 0;
    char c = 0;
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
            return STR_AND_LEN_OF(str, size, is_float(str, size),
                                  has_exponent(str, size));
        }
        ++size;
    }
    return NULL_STR_AND_LEN_TUPLE;
}

str_and_len_tuple_t parse_number(FILE *f, size_t *pos)
{
    if (!f || !pos)
    {
        return NULL_STR_AND_LEN_TUPLE;
    }

    // Obtains the length of the value
    // -1 because we already read the first digit (or sign)
    size_t end_pos = *pos - 1;

    bool out = false;

    char c = 0;
    // end_pos is incremented for each character found to be part of a
    // number
    while (SEEK_AND_GET_CHAR(end_pos))
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
            out = true;
            break;
        }

        if (out)
        {
            break;
        }
    }

    size_t len = end_pos - *pos;
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
    if (fseek(f, *pos - 1, SEEK_SET))
    {
        free(str);
        return NULL_STR_AND_LEN_TUPLE;
    }
    fread(str, sizeof(char), len, f);

    *pos += len - 1;
    return STR_AND_LEN_OF(str, len, is_float(str, len), has_exponent(str, len));
}

size_t parse_boolean_buff(char *buff, size_t *idx)
{
    if (!buff || !idx)
    {
        return 0;
    }

    size_t end_idx = *idx;
    size_t size = 0;
    char c = 0;
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

size_t parse_boolean(FILE *f, size_t *pos)
{
    if (!f || !pos)
    {
        return 0;
    }

    // -1 because we already read the first character
    size_t end_pos = *pos - 1;

    char c = 0;
    // end_pos is incremented for each character found to be part of a
    // boolean
    while (SEEK_AND_GET_CHAR(end_pos) && !IS_END_CHAR(c))
    {
    }
    size_t len = end_pos - *pos;
    *pos += len - 1;
    return len;
}

size_t get_nb_elts_array_buff(char *buff, size_t idx)
{
    if (!buff || buff[idx] == ']')
    {
        return 0;
    }

    size_t nb_elts = 0;

    // Counts the number of nested arrays/dicts
    u64 array_count = 1;
    u64 dict_count = 0;

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
            idx += get_str_len_buff(buff, idx);
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
            idx += get_num_len_buff(buff, idx);
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

size_t get_nb_elts_array(FILE *f, size_t pos)
{
    if (!f || (!fseek(f, pos++, SEEK_SET) && fgetc(f) == ']'))
    {
        return 0;
    }

    size_t nb_elts = 0;

    // Counts the number of arrays/dicts nesting at the current position
    u64 is_in_array = 1;
    u64 is_in_dict = 0;

    bool is_in_string = false;
    bool is_backslashing = false;
    bool comma_encountered = false;

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
            comma_encountered = true;
        }

        // If we are not in a string or if the string just ended
        if (!is_in_string || (is_in_string && c == '"' && !is_backslashing))
        {
            switch (c)
            {
            case '"':
                is_in_string = !is_in_string;
                break;

            case '[':
                ++is_in_array;
                break;

            case ']':
                --is_in_array;
                break;

            case '{':
                ++is_in_dict;
                break;

            case '}':
                --is_in_dict;
                break;

            case ',':
                if (!is_in_dict && is_in_array == 1)
                {
                    ++nb_elts;
                }
                break;
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

size_t get_nb_elts_dict_buff(char *buff, size_t idx)
{
    if (!buff || idx >= MAX_READ_BUFF_SIZE || buff[idx] == '}')
    {
        return 0;
    }

    size_t nb_elts = 0;

    // Counts the number of arrays/dicts nesting at the current position
    u64 dict_count = 1;
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
            idx += get_str_len_buff(buff, idx);
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
            idx += get_num_len_buff(buff, idx);
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

size_t get_nb_elts_dict(FILE *f, size_t pos)
{
    if (!f || (!fseek(f, pos++, SEEK_SET) && fgetc(f) == '}'))
    {
        return 0;
    }

    size_t nb_elts = 0;

    // Used for the case where the dict contains only one element, and so
    // does not contain a ','
    // It can only be 0 or 1 but is stored as a size_t because it is compared to
    // another size_t, so it will be cast to this size anyway
    size_t single_elt_found = 0;

    // Counts the number of arrays/dicts nesting at the current position
    u64 is_in_dict = 1;
    u64 is_in_array = 0;

    bool is_in_string = false;
    bool is_backslashing = false;

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
            switch (c)
            {
            case '"':
                is_in_string = !is_in_string;
                single_elt_found = 1;
                break;

            case '[':
                ++is_in_array;
                break;

            case ']':
                --is_in_array;
                break;

            case '{':
                ++is_in_dict;
                break;

            case '}':
                --is_in_dict;
                break;

            case ',':
                if (!is_in_array && is_in_dict == 1)
                {
                    ++nb_elts;
                }
                break;
            }
        }

        if (!is_in_dict)
        {
            break;
        }
    }
    return nb_elts == 0 ? single_elt_found : nb_elts + 1;
}

size_t get_nb_chars_in_array(FILE *f, size_t pos)
{
    if (!f)
    {
        return 0;
    }

    size_t nb_chars = 0;

    // Counts the number of arrays/dicts nesting at the current position
    u64 is_in_array = 1;
    u64 is_in_dict = 0;

    bool is_in_string = false;
    bool is_backslashing = false;

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
            switch (c)
            {
            case '"':
                is_in_string = !is_in_string;
                break;

            case '[':
                ++is_in_array;
                break;

            case ']':
                --is_in_array;
                break;

            case '{':
                ++is_in_dict;
                break;

            case '}':
                --is_in_dict;
                break;
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

size_t get_nb_chars_in_dict(FILE *f, size_t pos)
{
    if (!f)
    {
        return 0;
    }

    size_t nb_chars = 0;

    // Counts the number of arrays/dicts nesting at the current position
    u64 is_in_dict = 1;
    u64 is_in_array = 0;

    bool is_in_string = false;
    bool is_backslashing = false;

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
            switch (c)
            {
            case '"':
                is_in_string = !is_in_string;
                break;

            case '[':
                ++is_in_array;
                break;

            case ']':
                --is_in_array;
                break;

            case '{':
                ++is_in_dict;
                break;

            case '}':
                --is_in_dict;
                break;
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
