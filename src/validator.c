#include "validator.h"

/**
** \brief Called after encountering a '+' or '-' sign, or any digit.
**        Starts from the sign or digit that started the number
*/
char is_number_valid(char *buff, unsigned long *idx)
{
    if (!buff || !idx)
    {
        return 0;
    }

    unsigned long i = *idx - 1;
    char nb_inc_idx = 0;
    char c = 0;
    char prev_c = 0;
    while ((c = buff[i++]))
    {
        // Sign not preceded by an exponent
        if (nb_inc_idx > 0 && (c == '+' || c == '-')
            && !(prev_c == 'e' || prev_c == 'E'))
        {
            return 0;
        }

        // Exponent not followed by a digit or a sign
        if ((c == 'e' || c == 'E')
            && !((buff[i] >= '0' && buff[i] <= '9') || buff[i] == '+'
                 || buff[i] == '-'))
        {
            return 0;
        }

        // Floating point dot or sign not followed by a digit
        if ((c == '.' || c == '+' || c == '-')
            && !(buff[i] >= '0' && buff[i] <= '9'))
        {
            return 0;
        }
        ++nb_inc_idx;

        if (c == ',' || c == '\n')
        {
            break;
        }
        prev_c = c;
    }
    *idx += nb_inc_idx - 1;
    return 1;
}

/**
** \brief Checks if the booleans and nulls are spelled correctly, if the numbers
**        are valid, if the quotes are in even number, and brackets and curly
**        brackets have a matching number of opening and closing
*/
char check_bools_nulls_numbers_counts(char *buff, unsigned long buff_len,
                                      char is_dict)
{
    if (!buff)
    {
        return 0;
    }

    unsigned long nb_quotes = 0;
    unsigned long nb_opened_curly_brackets = is_dict;
    unsigned long nb_opened_brackets = !is_dict;
    unsigned long nb_closed_curly_brackets = 0;
    unsigned long nb_closed_brackets = 0;

    char is_in_string = 0;

    char c = 0;
    char prev_c = 0;
    unsigned long i = 1; // We already read the first character of the file
    while ((c = buff[i++]))
    {
        if (is_in_string)
        {
            if (c == '"' && prev_c != '\\') // String end
            {
                is_in_string = 0;
                ++nb_quotes;
                prev_c = c;
            }
            continue;
        }

        switch (c)
        {
        case '"':
            if (!is_in_string)
            {
                ++is_in_string;
                ++nb_quotes;
            }
            break;

        case 't':
            if (i + 3 < buff_len)
            {
                if (!(buff[i++] == 'r' && buff[i++] == 'u' && buff[i++] == 'e'))
                {
                    return 0;
                }
            }
            break;

        case 'f':
            if (i + 4 < buff_len)
            {
                if (!(buff[i++] == 'a' && buff[i++] == 'l' && buff[i++] == 's'
                      && buff[i++] == 'e'))
                {
                    return 0;
                }
            }
            break;

        case 'n':
            if (i + 3 < buff_len)
            {
                if (!(buff[i++] == 'u' && buff[i++] == 'l' && buff[i++] == 'l'))
                {
                    return 0;
                }
            }
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
            if (!is_number_valid(buff, &i))
            {
                return 0;
            }
            break;

        case '{':
            ++nb_opened_curly_brackets;
            break;
        case '[':
            ++nb_opened_brackets;
            break;
        case '}':
            ++nb_closed_curly_brackets;
            break;
        case ']':
            ++nb_closed_brackets;
            break;

        case ':':
        case ',':
        case '\n':
        case '\t':
        case ' ':
            break;

        default:
            // The character that is not part of the json syntax, which means
            // invalid json
            printf("Found invalid character: '%c' (%d)", c, c);
            return 0;
        }
        prev_c = c;
    }
    return nb_quotes % 2 == 0
        && nb_opened_curly_brackets == nb_closed_curly_brackets
        && nb_opened_brackets == nb_closed_brackets;
}

char check_arrays_and_dicts(char *buff)
{
    if (!buff)
    {
        return 0;
    }
    return 1;
}

char is_json_valid_buff(char *buff, unsigned long buff_len, char is_dict)
{
    if (!buff)
    {
        return 0;
    }
    return check_bools_nulls_numbers_counts(buff, buff_len, is_dict);
}

// TODO: Implement
char is_json_valid(FILE *f)
{
    if (!f)
    {
        return 0;
    }
    return 1;
}
