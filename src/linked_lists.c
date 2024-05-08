#include "linked_lists.h"

#include <stdio.h>
#include <stdlib.h>

size_t append_key(key_control_t *kc, const char *key)
{
    if (kc == NULL || key == NULL)
    {
        return 0;
    }

    // Case where there is no element
    if (kc->head == NULL)
    {
        kc->head = calloc(1, sizeof(struct key_array_link));
        if (kc->head == NULL)
        {
            return 0;
        }
        kc->head->keys[kc->idx++] = key;
    }
    // Case where the current head array is full
    else if (kc->idx == ARRAY_LEN)
    {
        struct key_array_link *nkl = calloc(1, sizeof(struct key_array_link));
        if (nkl == NULL)
        {
            return 0;
        }
        struct key_array_link *tmp = kc->head;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = nkl;
        kc->idx = 0;
        nkl->keys[kc->idx++] = key;
    }
    else
    {
        struct key_array_link *tmp = kc->head;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->keys[kc->idx++] = key;
    }
    ++kc->nb_keys;
    return kc->idx;
}

void print_keys(key_control_t *kc)
{
    if (kc == NULL || kc->head == NULL || kc->nb_keys == 0)
    {
        return;
    }

    struct key_array_link *tmp = kc->head;
    // Iterates over the arrays
    while (tmp != NULL)
    {
        printf("[ ");
        // Iterates over an array
        for (size_t i = 0; i < (tmp->next == NULL ? kc->idx : ARRAY_LEN); ++i)
        {
            printf("\"%s\"", tmp->keys[i]);
            if (i != (tmp->next == NULL ? kc->idx : ARRAY_LEN) - 1)
            {
                printf(", ");
            }
        }
        printf(" ]\n");
        tmp = tmp->next;
    }
}

void destroy_key_control(key_control_t *kc)
{
    if (kc == NULL)
    {
        return;
    }

    struct key_array_link *tmp = kc->head;
    while (tmp != NULL)
    {
        struct key_array_link *to_del = tmp;
        tmp = tmp->next;
        free(to_del);
    }
    free(kc);
}
