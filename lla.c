#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_LEN 8
#define ERROR_INT ((Int){ .type = 0, .value = 0 })

typedef struct
{
    char type;
    int value;
} Int;

typedef struct value_link
{
    Int values[ARRAY_LEN];
    unsigned insert_index;
    struct value_link *next;
} ValueLink;

typedef struct
{
    unsigned size;
    ValueLink *head;
    ValueLink *tail;
} Array;

void add(Array *a, int value)
{
    if (!a)
    {
        return;
    }

    if (!a->head)
    {
        ValueLink *vl = calloc(1, sizeof(ValueLink));
        if (!vl)
        {
            return;
        }
        vl->values[vl->insert_index++] = (Int){ .type = 1, .value = value };
        a->head = vl;
        a->tail = vl;
    }
    else
    {
        if (!a->tail)
        {
            return;
        }

        if (a->tail->insert_index < ARRAY_LEN)
        {
            a->tail->values[a->tail->insert_index++] =
                (Int){ .type = 1, .value = value };
        }
        else
        {
            ValueLink *vl = calloc(1, sizeof(ValueLink));
            if (!vl)
            {
                return;
            }
            vl->values[vl->insert_index++] = (Int){ .type = 1, .value = value };
            a->tail->next = vl;
            a->tail = vl;
        }
    }
    ++a->size;
}

Int get(Array *a, unsigned index)
{
    if (!a || index >= a->size)
    {
        return ERROR_INT;
    }

    ValueLink *link = a->head;
    unsigned non_null_values = 0;
    while (link)
    {
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            if (non_null_values == index)
            {
                return link->values[i];
            }
            if (link->values[i].type != 0)
            {
                ++non_null_values;
            }
        }
        link = link->next;
    }
    return ERROR_INT;
}

void array_remove(Array *a, unsigned index)
{
    if (!a || index >= a->size)
    {
        return;
    }

    ValueLink *link = a->head;
    unsigned non_null_values = 0;
    while (link)
    {
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            if (non_null_values == index)
            {
                link->values[i].type = 0;
                --a->size;
                return;
            }
            if (link->values[i].type != 0)
            {
                ++non_null_values;
            }
        }
        link = link->next;
    }
}

void print_array(Array *a)
{
    if (!a)
    {
        return;
    }

    ValueLink *link = a->head;
    while (link)
    {
        printf("[");
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            if (link->values[i].type != 0)
            {
                printf("%d, ", link->values[i].value);
            }
            else
            {
                printf(" , ");
            }
        }
        printf("] -> ");
        link = link->next;
    }
    printf("%u\n", a->size);
}

void copy_array(Int *src, Int *dest)
{
    if (!src || !dest)
    {
        return;
    }

    for (unsigned i = 0; i < ARRAY_LEN; ++i)
    {
        dest[i] = src[i];
    }
}

void defragment(Array *a)
{
    if (!a)
    {
        return;
    }

    Int tmps[ARRAY_LEN] = {};
    unsigned tmps_insert_idx = 0;
    ValueLink *link_to_fill = a->head;
    ValueLink *prev_link_to_fill = a->head;

    ValueLink *link = a->head;
    while (link)
    {
        for (unsigned i = 0; i < ARRAY_LEN; ++i)
        {
            // The temp array is filled, so we can put the defragmented array
            // back inside it
            if (tmps_insert_idx == ARRAY_LEN)
            {
                if (!link_to_fill)
                {
                    link_to_fill = calloc(1, sizeof(ValueLink));
                    if (!link_to_fill)
                    {
                        return;
                    }
                    prev_link_to_fill->next = link_to_fill;
                }

                // Copies the temp array to the base Array
                copy_array(tmps, link_to_fill->values);
                tmps_insert_idx = 0;
                prev_link_to_fill = link_to_fill;
                link_to_fill = link_to_fill->next;
            }

            if (link->values[i].type != 0)
            {
                tmps[tmps_insert_idx++] = link->values[i];
            }
        }
        link = link->next;
    }

    if (tmps_insert_idx)
    {
        copy_array(tmps, link_to_fill->values);
        a->tail = link_to_fill;
        link_to_fill->next = 0;
        link_to_fill = link_to_fill->next;
        while (link_to_fill)
        {
            ValueLink *tmp = link_to_fill;
            link_to_fill = link_to_fill->next;
            free(tmp);
        }
    }
}

int main(void)
{
    Array *a = calloc(1, sizeof(Array));
    add(a, 1);
    add(a, 2);
    add(a, 3);
    add(a, 4);
    add(a, 5);
    add(a, 6);
    add(a, 7);
    add(a, 8);
    add(a, 9);
    add(a, 10);
    print_array(a);
    array_remove(a, 6);
    print_array(a);
    array_remove(a, 2);
    print_array(a);
    array_remove(a, 4);
    print_array(a);
    array_remove(a, 9);
    print_array(a);
    defragment(a);
    print_array(a);
    free(a);
    return 0;
}
