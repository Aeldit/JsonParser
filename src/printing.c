#include "printing.h"

#include <stdio.h>
#include <stdlib.h>

void array_print_indent(json_array_st *ja, char indent, char from_list)
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
            switch (ja->elts[i].type)
            {
            case TYPE_STR:
                printf("%s\t\"%s\"", tabs, (char *)ja->elts[i].value);
                break;
            case TYPE_NUM:
                printf("%s\t%ld", tabs, *(long *)ja->elts[i].value);
                break;
            case TYPE_ARR:
                array_print_indent(ja->elts[i].value, indent + 1, 1);
                break;
            case TYPE_OBJ:
                j = ja->elts[i].value;
                printf("%s\t", tabs);
                if (j == NULL)
                {
                    printf("{}");
                }
                else
                {
                    print_json_rec(j->pairs, indent + 1);
                }
                break;
            case TYPE_BOOL:
                printf("%s\t%s", tabs,
                       *(char *)ja->elts[i].value ? "true" : "false");
                break;
            case TYPE_NULL:
                printf("%s\tnull", tabs);
                break;
            default:
                break;
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

void array_print(json_array_st *ja)
{
    array_print_indent(ja, 1, 0);
    printf("\n");
}

void print_json_rec(pair_control_st *ctrl, char indent)
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

    struct pair_link *array = ctrl->head;
    printf("{\n");
    // Iterates over the arrays
    while (array != NULL)
    {
        // Iterates over an array
        for (size_t i = 0; i < (array->next == NULL ? ctrl->idx : ARRAY_LEN);
             ++i)
        {
            if (array->pairs[i] != NULL)
            {
                const char *key = array->pairs[i]->key;
                long num = 0;
                char boolean = 0;
                json_array_st *l = NULL;
                json_dict_st *j = NULL;
                switch (array->pairs[i]->type)
                {
                case TYPE_STR:
                    printf("%s\t\"%s\": \"%s\"", tabs, key,
                           (const char *)array->pairs[i]->value);
                    break;
                case TYPE_NUM:
                    num = *(long *)array->pairs[i]->value;
                    printf("%s\t\"%s\": %ld", tabs, key, num);
                    break;
                case TYPE_OBJ:
                    j = array->pairs[i]->value;
                    printf("%s\t\"%s\": ", tabs, key);
                    if (j == NULL)
                    {
                        printf("{}");
                    }
                    else
                    {
                        print_json_rec(j->pairs, indent + 1);
                    }
                    break;
                case TYPE_ARR:
                    l = array->pairs[i]->value;
                    printf("%s\t\"%s\": ", tabs, key);
                    array_print_indent(l, indent + 1, 0);
                    break;
                case TYPE_BOOL:
                    boolean = *(char *)array->pairs[i]->value;
                    printf("%s\t\"%s\": %s", tabs, key,
                           boolean ? "true" : "false");
                    break;
                case TYPE_NULL:
                    printf("%s\t\"%s\": null", tabs, key);
                    break;
                default:
                    break;
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

void print_json(pair_control_st *ctrl)
{
    print_json_rec(ctrl, 1);
}
