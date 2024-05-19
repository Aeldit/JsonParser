#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "files_handling.h"

json_dict_st *parse(char *file)
{
    struct fc_control *fcc = read_file(file);
    if (fcc == NULL)
    {
        return NULL;
    }

    json_dict_st *jd = init_dict();
    struct states s = (struct states){ 0 };

    char buff[WRITE_BUFF_LEN] = { 0 };
    unsigned short buff_idx = 0;

    char prev_c = '\0';

    // Possible elements
    char *key = NULL;
    /*char *str = NULL;
    long num = 0;
    json_dict_control_st *jc = NULL;
    list_control_st *l = NULL;
    char bool = 0;*/

    struct file_content *tmp = fcc->head;
    while (tmp != NULL)
    {
        for (int i = 0; i < READ_BUFF_LEN; ++i)
        {
            // If the buffer is full
            if (buff_idx == WRITE_BUFF_LEN)
            {
                buff_idx = 0;
                // TODO: Add elts
            }

            switch (tmp->buffer[i])
            {
            case BEGIN_OBJECT:
                ++s.is_in_json;
                break;
            case END_OBJECT:
                --s.is_in_json;
                break;
            case BEGIN_ARRAY:
                ++s.is_in_array;
                break;
            case END_ARRAY:
                --s.is_in_array;
                break;
            case ':':
                if (!s.is_in_string)
                {
                    s.is_waiting_value = 1;
                }
                break;
            case ',':
                if (!s.is_in_string)
                {
                    s.is_waiting_key = 1;
                }
                else if (s.is_waiting_value)
                {
                    s.is_waiting_value = 0;
                }
                break;
            case '"':
                if (prev_c != '\\')
                {
                    if (s.is_in_string)
                    {
                        s.is_in_string = 0;
                        // buff[buff_idx] = '\0';
                        printf("%d %lu\n", buff[0], strlen(buff));
                        key = calloc(strlen(buff), sizeof(char *));
                        if (key == NULL)
                        {
                            return NULL;
                        }
                    }
                    else if (s.is_waiting_key)
                    {
                        s.is_waiting_key = 0;
                        s.is_in_string = 1;
                    }
                }
                break;
            default:
                printf("%c ", tmp->buffer[i]);
                buff[buff_idx] = tmp->buffer[i];
                printf("%c ", buff[buff_idx]);
                break;
            }
            prev_c = buff[buff_idx++];
        }
        tmp = tmp->next;
    }
    destroy_fc_control(fcc);
    return jd;
}
