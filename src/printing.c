#include "printing.h"

#include <stdio.h>
#include <stdlib.h>

void print_array_indent(json_array_st *ja, char indent, char from_list)
{
    // Obtains the number of tab characters that will be printed
    char *tabs = calloc(indent, sizeof(char));
    if (tabs == NULL)
    {
        return;
    }
    for (int i = 0; i < indent - 1; ++i)
    {
        tabs[i] = '\t';
    }
    tabs[indent - 1] = '\0';

    if (ja == NULL || ja->head == NULL || ja->size == 0)
    {
        printf("%s[]", from_list ? tabs : "");
        free(tabs);
        return;
    }

    printf("%s[\n", from_list ? tabs : "");

    struct link_value *link = ja->head;
    uint64_t ctr = 0;
    while (link != NULL)
    {
        for (uint64_t i = 0; i < BASE_ARRAY_LEN; ++i, ++ctr)
        {
            void *value = link->elts[i].value;
            unsigned char type = link->elts[i].type;

            // FIX: '(null)'s appearing out of nowhere
            // printf("\n\nis null ? %d\n\n", link->elts[i].value == NULL);

            if (type == TYPE_STR)
            {
                printf("%s\t\"%s\"", tabs, (char *)value);
            }
            else if (type == TYPE_INT)
            {
                printf("%s\t%ld", tabs, *(long *)value);
            }
            else if (type == TYPE_DOUBLE)
            {
                printf("%s\t%f", tabs, *(double *)value);
            }
            else if (type == TYPE_NULL)
            {
                printf("%s\tnull", tabs);
            }
            else if (type == TYPE_BOOL)
            {
                printf("%s\t%s", tabs, *(char *)value ? "true" : "false");
            }
            else if (type == TYPE_ARR)
            {
                print_array_indent(value, indent + 1, 1);
            }
            else if (type == TYPE_DICT)
            {
                print_json_indent(value, indent + 1, 0);
            }
        }

        if (ctr != ja->size - 1)
        {
            printf(",\n");
        }
        link = link->next;
    }
    printf("\n%s]", tabs);
    free(tabs);
}

void print_array(json_array_st *ja)
{
    print_array_indent(ja, 1, 0);
    printf("\n");
}

void print_json_indent(json_dict_st *jd, char indent, char from_dict)
{
    char *tabs = calloc(indent + 1, sizeof(char));
    if (tabs == NULL)
    {
        return;
    }
    for (int i = 0; i < indent - 1; ++i)
    {
        tabs[i] = '\t';
    }

    if (jd == NULL || jd->head == NULL || jd->size == 0)
    {
        printf("%s{}", from_dict ? "" : tabs);
        free(tabs);
        return;
    }

    printf("%s{\n", from_dict ? "" : tabs);

    struct link_item *link = jd->head;
    uint64_t ctr = 0;
    while (link != NULL)
    {
        for (uint64_t i = 0; i < BASE_ARRAY_LEN; ++i, ++ctr)
        {
            char *key = link->elts[i].key;
            void *value = link->elts[i].value;
            unsigned char type = link->elts[i].type;

            if (type == TYPE_STR)
            {
                printf("%s\t\"%s\": \"%s\"", tabs, key, (const char *)value);
            }
            else if (type == TYPE_INT)
            {
                printf("%s\t\"%s\": %ld", tabs, key, *(long *)value);
            }
            else if (type == TYPE_DOUBLE)
            {
                printf("%s\t\"%s\": %f", tabs, key, *(double *)value);
            }
            else if (type == TYPE_NULL)
            {
                printf("%s\t\"%s\": null", tabs, key);
            }
            else if (type == TYPE_BOOL)
            {
                printf("%s\t\"%s\": %s", tabs, key,
                       *(char *)value ? "true" : "false");
            }
            else if (type == TYPE_ARR)
            {
                printf("%s\t\"%s\": ", tabs, key);
                print_array_indent(link->elts[i].value, indent + 1, 0);
            }
            else if (type == TYPE_DICT)
            {
                printf("%s\t\"%s\": ", tabs, key);
                print_json_indent(value, indent + 1, 1);
            }
        }

        if (ctr != jd->size - 1)
        {
            printf(",\n");
        }
        link = link->next;
    }
    printf("\n%s}", tabs);
    if (indent == 1)
    {
        printf("\n");
    }
    free(tabs);
}

void print_json(json_dict_st *jd)
{
    print_json_indent(jd, 1, 0);
}
