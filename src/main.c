#include <stdio.h>
#include <stdlib.h>

#include "json.h"
#include "lists/generic_lists.h"
#include "lists/linked_lists.h"

int main(void)
{
    json_dict_st *j = init_dict();

    add_str(j, "t", "a");
    add_str(j, "te", "b");
    add_str(j, "tes", "c");
    add_str(j, "test", "d");
    add_str(j, "testi", "e");
    add_str(j, "testin", "f");
    add_str(j, "testing", "g");

    for (long i = 0; i < 10; ++i)
    {
        add_num(j, "a", i);
    }

    add_bool(j, "bool1", 0);
    add_bool(j, "bool2", 1);

    add_null(j, "testing_nulls");

    json_dict_st *jd = init_dict();

    add_str(jd, "t", "a");
    add_str(jd, "te", "b");
    add_str(jd, "tes", "c");
    add_str(jd, "test", "d");
    add_str(jd, "testi", "e");
    add_str(jd, "testin", "f");
    add_str(jd, "testing", "g");

    add_json_dict(jd, "dict", j);

    for (long i = 0; i < 10; ++i)
    {
        add_num(jd, "a", i);
    }

    add_bool(jd, "bool1", 0);
    add_bool(jd, "bool2", 1);

    add_null(jd, "testing_nulls");

    generic_list_st *e = calloc(1, sizeof(generic_list_st));
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

    generic_list_st *l = calloc(1, sizeof(generic_list_st));
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

    printf("\nexists : %s\n", key_exists(jd, "array") ? "true" : "false");

    destroy_dict(jd);
    return 0;
}
