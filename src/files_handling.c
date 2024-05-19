#include "files_handling.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
struct file_content *init_file_content(void)
{
    struct file_content *fc = calloc(1, sizeof(struct file_content));
    return fc == NULL ? NULL : fc;
}

struct fc_control *init_fc_control(void)
{
    struct fc_control *fcc = calloc(1, sizeof(struct fc_control));
    if (fcc == NULL)
    {
        return NULL;
    }

    struct file_content *fc = init_file_content();
    if (fc == NULL)
    {
        free(fcc);
        return NULL;
    }

    fcc->nb_buffers = 1;
    fcc->head = fc;
    fcc->tail = fc;
    return fcc;
}

void add_fc_to_fc_control(struct fc_control *fcc, struct file_content *fc)
{
    if (fcc == NULL || fc == NULL)
    {
        return;
    }

    fcc->tail->next = fc;
    fcc->tail = fc;
    ++fcc->nb_buffers;
}

void destroy_fc_control(struct fc_control *fcc)
{
    if (fcc == NULL)
    {
        return;
    }

    if (fcc->head == NULL)
    {
        free(fcc);
        return;
    }

    while (fcc->head != NULL)
    {
        struct file_content *tmp = fcc->head;
        fcc->head = fcc->head->next;
        free(tmp);
    }
    free(fcc);
}

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
        current_fc->buffer[buff_idx++] = c;
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
        printf("%s", tmp->buffer);
        tmp = tmp->next;
    }
}
