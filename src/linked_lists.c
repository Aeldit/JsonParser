#include "linked_lists.h"

#include <stdio.h>
#include <stdlib.h>

/**
** \brief  Appends the given key to the keys array
** \return The index of the added key + 1 (0 is the error code), so we have to
**         make sure later to substract 1 to the result of this function
*/
size_t append_key(key_control_t *kc, const char *key)
{
    if (kc == NULL || key == NULL)
    {
        return 0;
    }

    // Case where there is no element
    if (kc->tail == NULL)
    {
        kc->tail = calloc(1, sizeof(struct key_array_link));
        if (kc->tail == NULL)
        {
            return 0;
        }
    }
    // Case where the current head array is full
    else if (kc->idx == ARRAY_LEN)
    {
        struct key_array_link *nkl = calloc(1, sizeof(struct key_array_link));
        if (nkl == NULL)
        {
            return 0;
        }
        nkl->prev = kc->tail;
        kc->tail = nkl;
        kc->idx = 0;
    }
    kc->tail->keys[kc->idx++] = key;
    ++kc->nb_keys;
    return kc->idx;
}

void print_keys(key_control_t *kc)
{
    if (kc == NULL || kc->tail == NULL || kc->nb_keys == 0)
    {
        return;
    }

    struct key_array_link *tmp = kc->tail;
    char last = 1;
    // Iterates over the arrays
    while (tmp != NULL)
    {
        printf("[ ");
        // Iterates over an array
        for (size_t i = 0; i < (last ? kc->idx : ARRAY_LEN); ++i)
        {
            printf("\"%s\"", tmp->keys[i]);
            if (i != (last ? kc->idx : ARRAY_LEN) - 1)
            {
                printf(", ");
            }

            if (last)
            {
                last = 0;
            }
        }
        printf(" ]\n");
        tmp = tmp->prev;
    }
}

void destroy_key_control(key_control_t *kc)
{
    if (kc == NULL)
    {
        return;
    }

    struct key_array_link *tmp = kc->tail;
    while (tmp != NULL)
    {
        struct key_array_link *to_del = tmp;
        tmp = tmp->prev;
        free(to_del);
    }
    free(kc);
}
