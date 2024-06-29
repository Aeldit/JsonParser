#include "parser2.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }
    /*json_dict_st *j = init_dict();

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

    add_num(jd, "number", 1596843165152);

    json_array_st *e = calloc(1, sizeof(json_array_st));
    if (e == NULL)
    {
        return 1;
    }
    e->size = 3;
    array_append(
        e, (struct list_elt){ .type = TYPE_NUM, .value = append_num(jd, 150) });
    array_append(
        e,
        (struct list_elt){ .type = TYPE_STR,
                           .value = append_str(jd, "aaaaaaaaaaaaaaa") });
    array_append(e, (struct list_elt){ .type = TYPE_NULL, .value = NULL });

    json_array_st *l = calloc(1, sizeof(json_array_st));
    if (l == NULL)
    {
        return 1;
    }
    l->size = 4;
    array_append(
        l, (struct list_elt){ .type = TYPE_NUM, .value = append_num(jd, 150) });
    array_append(
        l,
        (struct list_elt){ .type = TYPE_STR,
                           .value = append_str(jd, "aaaaaaaaaaaaaaa") });
    array_append(l, (struct list_elt){ .type = TYPE_ARR, .value = e });
    array_append(l, (struct list_elt){ .type = TYPE_NULL, .value = NULL });
    add_array(jd, "array", l);

    print_json(jd->pairs);

    printf("\nexists : %s\n", key_exists(jd, "array") ? "true" : "false");

    typed_value_st tv = get_value(jd, "array");
    switch (tv.type)
    {
    case TYPE_STR:
        printf("%s\n", (char *)tv.value);
        break;
    case TYPE_NUM:
        printf("%lu\n", *(long *)tv.value);
        break;
    case TYPE_OBJ:
        print_json(((json_dict_st *)tv.value)->pairs);
        break;
    case TYPE_ARR:
        array_print((json_array_st *)tv.value);
        break;
    case TYPE_BOOL:
        printf("%s\n", *(char *)tv.value ? "true" : "false");
        break;
    case TYPE_NULL:
        printf("null\n");
        break;
    case TYPE_ERROR:
        puts("Error");
        break;
    }*/

    parse(argv[1]);
    // json_dict_st *jd = parse(argv[1]);

    // print_json(jd->pairs);

    // destroy_dict(jd);
    return 0;
}
