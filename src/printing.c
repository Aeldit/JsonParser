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

    if (ja == NULL || ja->elts == NULL || ja->size == 0)
    {
        printf("%s[]", from_list ? tabs : "");
    }
    else
    {
        printf("%s[\n", from_list ? tabs : "");

        json_dict_st *j = NULL;
        for (size_t i = 0; i < ja->size; ++i)
        {
            unsigned char type = ja->elts[i].type;

            if (type == TYPE_STR)
            {
                printf("%s\t\"%s\"", tabs, (char *)ja->elts[i].value);
            }
            else if (type == TYPE_INT)
            {
                printf("%s\t%ld", tabs, *(long *)ja->elts[i].value);
            }
            else if (type == TYPE_DOUBLE)
            {
                printf("%s\t%f", tabs, *(double *)ja->elts[i].value);
            }
            else if (type == TYPE_NULL)
            {
                printf("%s\tnull", tabs);
            }
            else if (type == TYPE_BOOL)
            {
                printf("%s\t%s", tabs,
                       *(char *)ja->elts[i].value ? "true" : "false");
            }
            else if (type == TYPE_ARR)
            {
                print_array_indent(ja->elts[i].value, indent + 1, 1);
            }
            else if (type == TYPE_DICT)
            {
                j = ja->elts[i].value;
                printf("%s\t", tabs);
                if (j->nb_items == 0)
                {
                    printf("{}");
                }
                else
                {
                    print_json_indent(j->items, indent + 1);
                }
            }

            if (i != ja->size - 1)
            {
                printf(",\n");
            }
        }
        printf("\n%s]", tabs);
    }
    free(tabs);
}

void print_array(json_array_st *ja)
{
    print_array_indent(ja, 1, 0);
    printf("\n");
}

void print_json_indent(item_control_st *ctrl, char indent)
{
    if (ctrl == NULL || ctrl->head == NULL)
    {
        return;
    }

    char *tabs = calloc(indent + 1, sizeof(char));
    if (tabs == NULL)
    {
        return;
    }
    for (int i = 0; i < indent - 1; ++i)
    {
        tabs[i] = '\t';
    }

    struct item_link *array = ctrl->head;
    printf("{\n");
    // Iterates over the arrays
    while (array != NULL)
    {
        // Iterates over an array
        size_t end_case = (array->next == NULL ? ctrl->idx : ARRAY_LEN);
        for (size_t i = 0; i < end_case; ++i)
        {
            struct item *pair = array->items[i];
            if (pair != NULL)
            {
                const char *key = pair->key;
                json_dict_st *j = NULL;

                unsigned char type = pair->type;

                // Prints the value in the correct form depending on its type
                if (type == TYPE_STR)
                {
                    printf("%s\t\"%s\": \"%s\"", tabs, key,
                           (const char *)pair->value);
                }
                else if (type == TYPE_INT)
                {
                    printf("%s\t\"%s\": %ld", tabs, key, *(long *)pair->value);
                }
                else if (type == TYPE_DOUBLE)
                {
                    printf("%s\t\"%s\": %f", tabs, key, *(double *)pair->value);
                }
                else if (type == TYPE_NULL)
                {
                    printf("%s\t\"%s\": null", tabs, key);
                }
                else if (type == TYPE_BOOL)
                {
                    printf("%s\t\"%s\": %s", tabs, key,
                           *(char *)pair->value ? "true" : "false");
                }
                else if (type == TYPE_ARR)
                {
                    printf("%s\t\"%s\": ", tabs, key);
                    print_array_indent(pair->value, indent + 1, 0);
                }
                else if (type == TYPE_DICT)
                {
                    j = pair->value;
                    printf("%s\t\"%s\": ", tabs, key);
                    if (j->nb_items == 0)
                    {
                        printf("{}");
                    }
                    else
                    {
                        print_json_indent(j->items, indent + 1);
                    }
                }

                // If not at the last element of the last array, we print a ','
                if ((array->next == NULL && i != ctrl->idx - 1)
                    || array->next != NULL)
                {
                    printf(",\n");
                }
            }
        }
        array = array->next;
    }
    printf("\n%s}", tabs);
    if (indent == 1)
    {
        printf("\n");
    }
    free(tabs);
}

void print_json(item_control_st *ctrl)
{
    print_json_indent(ctrl, 1);
}
