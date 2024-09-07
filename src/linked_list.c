#include "linked_list.h"

#include <stdlib.h>

typed_value_st *getValue(json_array_st *ja, uint64_t index)
{
    if (ja->head == NULL || index >= ja->size)
    {
        return NULL;
    }

    struct link_value *link = ja->head;
    uint64_t nb_encountered = 0;
    while (link != NULL)
    {
        for (unsigned char i = 0; i < BASE_ARRAY_LEN; ++i)
        {
            if (link->elts[i].type != TYPE_ERROR)
            {
                if (nb_encountered == index)
                {
                    return &link->elts[i];
                }
                ++nb_encountered;
            }
        }
        link = link->next;
    }
    return NULL;
}

item_st *getItem(json_dict_st *jd, uint64_t index)
{
    if (jd->head == NULL || index >= jd->size)
    {
        return NULL;
    }

    struct link_item *link = jd->head;
    uint64_t nb_encountered = 0;
    while (link != NULL)
    {
        for (unsigned char i = 0; i < BASE_ARRAY_LEN; ++i)
        {
            if (link->elts[i].type != TYPE_ERROR)
            {
                if (nb_encountered == index)
                {
                    return &link->elts[i];
                }
                ++nb_encountered;
            }
        }
        link = link->next;
    }
    return NULL;
}

void addValue(json_array_st *ja, typed_value_st value)
{
    if (value.type == TYPE_ERROR)
    {
        return;
    }

    if (ja->head == NULL)
    {
        ja->head = calloc(1, sizeof(struct link_value));
        ja->tail = ja->head;
    }

    if (ja->insert_idx >= BASE_ARRAY_LEN)
    {
        ja->tail->next = calloc(1, sizeof(struct link_value));
        ja->tail = ja->tail->next;
        ja->insert_idx = 0;
    }

    ja->tail->elts[ja->insert_idx++] = value;
    ++(ja->size);
}

void addItem(json_dict_st *jd, item_st value)
{
    if (jd == NULL || value.type == TYPE_ERROR)
    {
        return;
    }

    if (jd->head == NULL)
    {
        jd->head = calloc(1, sizeof(struct link_item));
        jd->tail = jd->head;
    }

    if (jd->insert_idx >= BASE_ARRAY_LEN)
    {
        jd->tail->next = calloc(1, sizeof(struct link_item));
        jd->tail = jd->tail->next;
        jd->insert_idx = 0;
    }

    jd->tail->elts[jd->insert_idx++] = value;
    ++(jd->size);
}

void removeValue(json_array_st *ja, uint64_t index)
{
    if (ja->head == NULL || index >= ja->size)
    {
        return;
    }

    struct link_value *link = ja->head;
    uint64_t nb_encountered = 0;
    char done = 0;
    while (link != NULL)
    {
        for (unsigned char i = 0; i < BASE_ARRAY_LEN; ++i)
        {
            if (link->elts[i].type != TYPE_ERROR)
            {
                if (nb_encountered == index)
                {
                    link->elts[i].type = TYPE_ERROR;
                    // delete_elt(link->elts[i]);
                    done = 1;
                    break;
                }
                ++nb_encountered;
            }
        }
        if (done)
        {
            break;
        }
        link = link->next;
    }
    --(ja->size);

    ++(ja->nb_deletion);
    if (ja->nb_deletion == BASE_ARRAY_LEN)
    {
        // defragment();
    }
}

void removeItem(json_dict_st *jd, uint64_t index)
{
    if (jd->head == NULL || index >= jd->size)
    {
        return;
    }

    struct link_item *link = jd->head;
    uint64_t nb_encountered = 0;
    char done = 0;
    while (link != NULL)
    {
        for (unsigned char i = 0; i < BASE_ARRAY_LEN; ++i)
        {
            if (link->elts[i].type != TYPE_ERROR)
            {
                if (nb_encountered == index)
                {
                    link->elts[i].type = TYPE_ERROR;
                    // delete_elt(link->elts[i]);
                    done = 1;
                    break;
                }
                ++nb_encountered;
            }
        }
        if (done)
        {
            break;
        }
        link = link->next;
    }
    --(jd->size);

    ++(jd->nb_deletion);
    if (jd->nb_deletion == BASE_ARRAY_LEN)
    {
        // defragment();
    }
}

void destroy_json_array(json_array_st *ja)
{
    if (ja == NULL)
    {
        return;
    }

    struct link_value *tmp = ja->head;
    while (tmp != NULL)
    {
        struct link_value *t = tmp;
        tmp = tmp->next;
        free(t);
    }
    free(ja);
}

void destroy_json_dict(json_dict_st *jd)
{
    if (jd == NULL)
    {
        return;
    }

    struct link_item *tmp = jd->head;
    while (tmp != NULL)
    {
        struct link_item *t = tmp;
        tmp = tmp->next;
        for (uint64_t i = 0; i < BASE_ARRAY_LEN; ++i)
        {
            free(t->elts[i].key);
        }
        free(t);
    }
    free(jd);
}
