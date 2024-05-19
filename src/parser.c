#include "parser.h"

#include "files_handling.h"

json_dict_st *parse(char *file)
{
    struct fc_control *fcc = read_file(file);
    if (fcc == NULL)
    {
        return NULL;
    }

    json_dict_st *jd = init_dict();
    struct states s = (struct states){ .is_in_json = 0 };

    struct file_content *tmp = fcc->head;
    while (tmp != NULL)
    {
        for (int i = 0; i < READ_BUFF_LEN; ++i)
        {
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
            }
        }
        tmp = tmp->next;
    }
    return jd;
}
