#include "parser.h"

#include <stdio.h>

#include "files_handling.h"

struct fc_control *read_file(char *path)
{
    FILE *f = fopen(path, "r");
    if (f == NULL)
    {
        return NULL;
    }

    struct fc_control *fcc = init_fc_control();
    if (fcc == NULL)
    {
        return NULL;
    }

    struct file_content *current_fc = fcc->head;

    int lines = 0;
    if (fseek(f, lines++, SEEK_SET) != 0)
    {
        destroy_fc_control(fcc);
        return NULL;
    }

    char c;
    int buff_idx = 0;
    // last_fc is used to get the last read file_content buffer (which is not
    // full)
    struct file_content *last_fc = NULL;
    while ((c = fgetc(f)) != EOF)
    {
        if (fseek(f, lines++, SEEK_SET) != 0)
        {
            break;
        }

        // If there is no more place in the buffer, we create another one
        if (buff_idx == READ_BUFF_LEN)
        {
            add_fc_to_fc_control(fcc, current_fc);
            // Creates a new link and assigns it to current_fc
            struct file_content *new_fc = init_file_content();
            if (new_fc == NULL)
            {
                break;
            }
            buff_idx = 0;
            current_fc = new_fc;
            last_fc = new_fc;
        }
        current_fc->payload[buff_idx++] = c;
    }
    add_fc_to_fc_control(fcc, last_fc);
    return fcc;
}

void print_file_content(struct fc_control *fcc)
{
    if (fcc == NULL)
    {
        return;
    }

    puts("");
    struct file_content *tmp = fcc->head;
    while (tmp != NULL)
    {
        printf("%s", tmp->payload);
        tmp = tmp->next;
    }
}
