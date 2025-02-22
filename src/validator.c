#include "validator.h"

#include <string.h>

/**
** \brief Called after encountering a '+' or '-' sign, or any digit
*/
char is_number_valid(char *buff, unsigned long *idx)
{
    if (!buff || !idx)
    {
        return 0;
    }

    unsigned long i = *idx;
    char nb_inc_idx = 0;
    char c = 0;
    char prev_c = 0;
    while ((c = buff[i++]))
    {
        if (c == ',')
        {
            break;
        }
        ++nb_inc_idx;

        // sign not preceded by an exponent
        if ((c == '+' || c == '-') && !(prev_c == 'e' || prev_c == 'E'))
        {
            return 0;
        }

        // exponent not followed by a digit or a sign
        if ((c == 'e' || c == 'E')
            && !((buff[i] >= '0' && buff[i] <= '9') || buff[i] == '+'
                 || buff[i] == '-'))
        {
            return 0;
        }

        // floating point dot or sign not followed by a digit
        if ((c == '.' || c == '+' || c == '-')
            && !(buff[i] >= '0' && buff[i] <= '9'))
        {
            return 0;
        }
        prev_c = c;
    }
    *idx += nb_inc_idx;
    return 1;
}

char is_json_valid_buff(char *buff)
{
    if (!buff)
    {
        return 0;
    }

    unsigned long buff_len = strlen(buff);

    unsigned long nb_quotes = 0;

    char is_in_string = 0;

    char c = 0;
    char prev_c = 0;
    unsigned long i = 0;
    while ((c = buff[i++]))
    {
        if (is_in_string)
        {
            if (c == '"' && prev_c != '\\') // String end
            {
                --is_in_string;
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
        case '[':
        case '}':
        case ']':
        case ':':
        case ',':
        case '\n':
        case '\t':
        case ' ':
            break;

        default:
            // character that is not part of the json syntax, which means
            // invalid json
            return 0;
        }
        prev_c = c;
    }
    return nb_quotes % 2 == 0;
}
