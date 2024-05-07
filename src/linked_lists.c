#include "linked_lists.h"

#include <stdlib.h>

#include "utils.h"

/**
** \brief Appends the given key to the keys array
** \return The index of the added key + 1 (0 is the error code)
*/
size_t append_key(key_control_t *kl, const char *key)
{
    if (kl == NULL || key == NULL)
    {
        return FAILURE;
    }

    // Case where there is no element
    if (kl->head == NULL)
    {
        kl->head = calloc(1, sizeof(struct key_array_link *));
        if (kl->head == NULL)
        {
            return 0;
        }
    }
    // Case where the current head array is full
    else if ((kl->nb_keys % ARRAY_LEN) == 0)
    {
        struct key_array_link *nkl = calloc(1, sizeof(struct key_array_link *));
        if (nkl == NULL)
        {
            return 0;
        }
        nkl->previous = kl->head;
        kl->head = nkl;
    }
    kl->head->keys[kl->idx++] = key;
    ++kl->nb_keys;
    return kl->idx;
}
