#include <stdio.h>
#include <stdlib.h>

#include "lists/generic_lists.h"
#include "lists/linked_lists.h"
#include "parser.h"

int main(void)
{
    json_dict_st *jd = init_dict();

    add_str(jd, "t", "a");
    add_str(jd, "te", "b");
    add_str(jd, "tes", "c");
    add_str(jd, "test", "d");
    add_str(jd, "testi", "e");
    add_str(jd, "testin", "f");
    add_str(jd, "testing", "g");

    for (long i = 0; i < 10; ++i)
    {
        add_num(jd, "a", i);
    }

    add_bool(jd, "bool1", 0);
    add_bool(jd, "bool2", 1);

    add_null(jd, "testing_nulls");

    struct generic_list *e = calloc(1, sizeof(struct generic_list));
    if (e == NULL)
    {
        return 1;
    }
    e->size = 3;
    append(e,
           (struct list_elt){ .type = TYPE_NUM,
                              .value = append_num(jd->numbers, 150) });
    append(e,
           (struct list_elt){ .type = TYPE_STR,
                              .value =
                                  append_str(jd->strings, "aaaaaaaaaaaaaaa") });
    append(e, (struct list_elt){ .type = TYPE_NULL, .value = NULL });

    struct generic_list *l = calloc(1, sizeof(struct generic_list));
    if (l == NULL)
    {
        return 1;
    }
    l->size = 4;
    append(l,
           (struct list_elt){ .type = TYPE_NUM,
                              .value = append_num(jd->numbers, 150) });
    append(l,
           (struct list_elt){ .type = TYPE_STR,
                              .value =
                                  append_str(jd->strings, "aaaaaaaaaaaaaaa") });
    append(l, (struct list_elt){ .type = TYPE_ARR, .value = e });
    append(l, (struct list_elt){ .type = TYPE_NULL, .value = NULL });
    add_list(jd, "array", l);

    print_json(jd->pairs);
    destroy_dict(jd);
    return 0;
}
