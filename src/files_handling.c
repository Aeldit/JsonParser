#include "files_handling.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
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

    fcc->nb_payloads = 1;
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
    ++fcc->nb_payloads;
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
