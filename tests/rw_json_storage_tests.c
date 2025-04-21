#include <criterion/criterion.h>

#include "../src/base_json_parser.h"
#include "../src/rw_json_parser.h"
#include "../src/rw_json_storage.h"

#define JSON_TESTS_FILE ("tests/test.json")

/*******************************************************************************
**                                 INIT_RW_ARRAY                              **
*******************************************************************************/
Test(rw_json_storage, init_rw_array_zero_size)
{
    size_t size   = 0;
    rw_array_t *a = init_rw_array();
    if (!a)
    {
        return;
    }

    cr_expect(
        a->size == size, "Expected the array size to be %zu, but got %zu", size,
        a->size
    );
    cr_expect(
        !a->head, "Expected a->head to be a null pointer, but got '%p'", a->head
    );

    destroy_rw_array(a);
}

Test(rw_json_storage, init_rw_array_normal_size)
{
    size_t size   = 5;
    rw_array_t *a = init_rw_array();
    if (!a)
    {
        return;
    }
    rw_array_add_bool(a, 1);
    rw_array_add_bool(a, 1);
    rw_array_add_bool(a, 1);
    rw_array_add_bool(a, 1);
    rw_array_add_bool(a, 1);

    cr_expect(
        a->size == size, "Expected the array size to be %zu, but got %zu", size,
        a->size
    );
    cr_expect(
        a->head, "Expected a->values to be a valid pointer, but it was null"
    );

    destroy_rw_array(a);
}

/*******************************************************************************
**                                 INIT_RW_DICT                               **
*******************************************************************************/
Test(rw_json_storage, init_rw_dict_zero_size)
{
    size_t size  = 0;
    rw_dict_t *d = calloc(1, sizeof(rw_dict_t));
    if (!d)
    {
        return;
    }

    cr_expect(
        d->size == size, "Expected the dict size to be %zu, but got %zu", size,
        d->size
    );
    cr_expect(
        !d->head, "Expected d->head to be a null pointer, but got '%p'", d->head
    );

    destroy_rw_dict(d);
}

Test(rw_json_storage, init_rw_dict_normal_size)
{
    size_t size  = 5;
    rw_dict_t *d = calloc(1, sizeof(rw_dict_t));
    if (!d)
    {
        return;
    }
    rw_dict_add_bool(d, string_nofree_of("1"), true);
    rw_dict_add_bool(d, string_nofree_of("2"), true);
    rw_dict_add_bool(d, string_nofree_of("3"), true);
    rw_dict_add_bool(d, string_nofree_of("4"), true);
    rw_dict_add_bool(d, string_nofree_of("5"), true);

    cr_expect(
        d->size == size, "Expected the dict size to be %zu, but got %zu", size,
        d->size
    );
    cr_expect(
        d->head, "Expected d->items to be a valid pointer, but it was null"
    );

    destroy_rw_dict(d);
}

/*******************************************************************************
**                                  ARRAY ADDS                                **
*******************************************************************************/
#define RW_ARRAY_ADD_TYPE_TEST_ADD_3_TYPE(rw_array_add_type, v1, v2, v3)       \
    size_t len    = 3;                                                         \
    rw_array_t *a = init_rw_array();                                           \
    if (!a)                                                                    \
    {                                                                          \
        return;                                                                \
    }                                                                          \
    rw_array_add_type(a, v1);                                                  \
    rw_array_add_type(a, v2);                                                  \
    rw_array_add_type(a, v3);                                                  \
    cr_expect(                                                                 \
        a->size == len,                                                        \
        "Expected the insert_index of the array to be %zu after the "          \
        "additions, but got '%zu'",                                            \
        len, a->size                                                           \
    );                                                                         \
    destroy_rw_array(a)

Test(rw_json_storage, rw_array_add_str_array_add_3)
{
    RW_ARRAY_ADD_TYPE_TEST_ADD_3_TYPE(
        rw_array_add_str, string_nofree_of("t1"), string_nofree_of("t3"),
        string_nofree_of("t3")
    );
}

Test(rw_json_storage, rw_array_add_long_array_add_3)
{
    RW_ARRAY_ADD_TYPE_TEST_ADD_3_TYPE(rw_array_add_long, 1, 2, 3);
}

Test(rw_json_storage, rw_array_add_double_array_add_3)
{
    RW_ARRAY_ADD_TYPE_TEST_ADD_3_TYPE(rw_array_add_double, 1.5, 2.6, 3.7);
}

Test(rw_json_storage, rw_array_add_explong_array_add_3)
{
    RW_ARRAY_ADD_TYPE_TEST_ADD_3_TYPE(
        rw_array_add_exp_long, EXP_LONG_OF(1, 2), EXP_LONG_OF(3, 4),
        EXP_LONG_OF(5, 6)
    );
}

Test(rw_json_storage, rw_array_add_expdouble_array_add_3)
{
    RW_ARRAY_ADD_TYPE_TEST_ADD_3_TYPE(
        rw_array_add_exp_double, EXP_DOUBLE_OF(1.5, 2),
        EXP_DOUBLE_OF(2.5005, 3), EXP_DOUBLE_OF(0.555589, 5)
    );
}

Test(rw_json_storage, rw_array_add_bool_array_add_3)
{
    RW_ARRAY_ADD_TYPE_TEST_ADD_3_TYPE(rw_array_add_bool, 0, 1, 0);
}

Test(rw_json_storage, rw_array_add_null_array_add_3)
{
    size_t len    = 3;
    rw_array_t *a = init_rw_array();
    if (!a)
    {
        return;
    }
    rw_array_add_null(a);
    rw_array_add_null(a);
    rw_array_add_null(a);
    cr_expect(
        a->size == len,
        "Expected the insert_index of the array to be %zu after the "
        "additions, "
        "but got '%zu'",
        len, a->size
    );
    destroy_rw_array(a);
}

/*******************************************************************************
**                                  DICT ADDS                                 **
*******************************************************************************/
#define RW_DICT_ADD_TYPE_TEST_ADD_3_TYPE(                                      \
    rw_dict_add_type, k1, k2, k3, v1, v2, v3                                   \
)                                                                              \
    size_t len   = 3;                                                          \
    rw_dict_t *d = calloc(1, sizeof(rw_dict_t));                               \
    if (!d)                                                                    \
    {                                                                          \
        return;                                                                \
    }                                                                          \
    rw_dict_add_type(d, k1, v1);                                               \
    rw_dict_add_type(d, k2, v2);                                               \
    rw_dict_add_type(d, k3, v3);                                               \
    cr_expect(                                                                 \
        d->size == len,                                                        \
        "Expected the insert_index of the dict to be %zu after the "           \
        "additions, but got '%zu'",                                            \
        len, d->size                                                           \
    );                                                                         \
    destroy_rw_dict(d)

Test(rw_json_storage, ro_dict_add_str_array_add_3)
{
    RW_DICT_ADD_TYPE_TEST_ADD_3_TYPE(
        rw_dict_add_str, string_nofree_of("k1"), string_nofree_of("k2"),
        string_nofree_of("k3"), string_nofree_of("t1"), string_nofree_of("t2"),
        string_nofree_of("t3")
    );
}

Test(rw_json_storage, ro_dict_add_long_array_add_3)
{
    RW_DICT_ADD_TYPE_TEST_ADD_3_TYPE(
        rw_dict_add_long, string_nofree_of("k1"), string_nofree_of("k2"),
        string_nofree_of("k3"), 1, 2, 45684898
    );
}

Test(rw_json_storage, ro_dict_add_double_array_add_3)
{
    RW_DICT_ADD_TYPE_TEST_ADD_3_TYPE(
        rw_dict_add_double, string_nofree_of("k1"), string_nofree_of("k2"),
        string_nofree_of("k3"), 0.1, 2.5468, 25.456848
    );
}

Test(rw_json_storage, ro_dict_add_explong_array_add_3)
{
    RW_DICT_ADD_TYPE_TEST_ADD_3_TYPE(
        rw_dict_add_exp_long, string_nofree_of("k1"), string_nofree_of("k2"),
        string_nofree_of("k3"), EXP_LONG_OF(1, 2), EXP_LONG_OF(3, 4),
        EXP_LONG_OF(5, 6)
    );
}

Test(rw_json_storage, ro_dict_add_expdouble_array_add_3)
{
    RW_DICT_ADD_TYPE_TEST_ADD_3_TYPE(
        rw_dict_add_exp_double, string_nofree_of("k1"), string_nofree_of("k2"),
        string_nofree_of("k3"), EXP_DOUBLE_OF(1.53, 45),
        EXP_DOUBLE_OF(-1.23, 4), EXP_DOUBLE_OF(0.002553, 5)
    );
}

Test(rw_json_storage, ro_dict_add_bool_array_add_3)
{
    RW_DICT_ADD_TYPE_TEST_ADD_3_TYPE(
        rw_dict_add_bool, string_nofree_of("k1"), string_nofree_of("k2"),
        string_nofree_of("k3"), 0, 1, 0
    );
}

Test(rw_json_storage, ro_dict_add_null_array_add_3)
{
    size_t len   = 3;
    rw_dict_t *d = calloc(1, sizeof(rw_dict_t));
    if (!d)
    {
        return;
    }
    rw_dict_add_null(d, string_nofree_of("k1"));
    rw_dict_add_null(d, string_nofree_of("k2"));
    rw_dict_add_null(d, string_nofree_of("k3"));
    cr_expect(
        d->size == len,
        "Expected the insert_index of the dict to be %zu after the "
        "additions, but got '%zu'",
        len, d->size
    );
    destroy_rw_dict(d);
}

/*******************************************************************************
**                                    GETS                                    **
*******************************************************************************/
Test(rw_json_storage, rw_array_get_empty)
{
    size_t idx    = 0;
    char buff[]   = "[]";
    rw_array_t *a = rw_parse_array(buff, &idx);
    if (!a)
    {
        return;
    }

    size_t i      = 5;
    rw_value_t *v = rw_array_get(a, i);
    if (!v)
    {
        destroy_rw_array(a);
        return;
    }

    cr_expect(
        v->type == T_ERROR,
        "Expected the value to be of type T_ERROR because we tried to "
        "get the value of index %zu from an array of size 0, but it wasn't",
        i
    );

    destroy_rw_array(a);
}

Test(rw_json_storage, rw_dict_get_empty)
{
    size_t idx   = 0;
    char buff[]  = "{}";
    rw_dict_t *d = rw_parse_dict(buff, &idx);
    if (!d)
    {
        return;
    }

    string_t key  = string_nofree_of("test");
    rw_item_t *it = rw_dict_get(d, key);
    if (!it)
    {
        destroy_rw_dict(d);
        return;
    }

    cr_expect(
        it->type == T_ERROR,
        "Expected the value to be of type T_ERROR because we tried to "
        "get the value of key %s on a dict of size 0, but it wasn't",
        key.str
    );

    destroy_rw_dict(d);
}

char *rw_get_type_as_str(u8 type)
{
    switch (type)
    {
    case T_ERROR:
        return "T_ERROR";
    case T_STR:
        return "T_STR";
    case T_LONG:
        return "T_LONG";
    case T_DOUBLE:
        return "T_DOUBLE";
    case T_EXP_LONG:
        return "T_EXP_LONG";
    case T_EXP_DOUBLE:
        return "T_EXP_DOUBLE";
    case T_BOOL:
        return "T_BOOL";
    case T_NULL:
        return "T_NULL";
    case T_ARR:
        return "T_ARR";
    case T_DICT:
        return "T_DICT";
    }
    return "\0";
}

void rw_array_get_type(char *buff, u8 expected_type)
{
    size_t idx    = 0;
    rw_array_t *a = rw_parse_array(buff, &idx);
    if (!a)
    {
        return;
    }

    rw_value_t *v = rw_array_get(a, 0);
    if (!v)
    {
        destroy_rw_array(a);
        return;
    }

    cr_expect(
        v->type == expected_type,
        "Expected the value to be of type %s (got '%s')",
        rw_get_type_as_str(expected_type), rw_get_type_as_str(v->type)
    );

    destroy_rw_array(a);
}

Test(rw_json_storage, rw_array_get_str)
{
    rw_array_get_type("[\"test\"]", T_STR);
}

Test(rw_json_storage, rw_array_get_long)
{
    rw_array_get_type("[1]", T_LONG);
}

Test(rw_json_storage, rw_array_get_double)
{
    rw_array_get_type("[1.5]", T_DOUBLE);
}

Test(rw_json_storage, rw_array_get_explong)
{
    rw_array_get_type("[1e5]", T_EXP_LONG);
}

Test(rw_json_storage, rw_array_get_expdouble)
{
    rw_array_get_type("[1.5e89]", T_EXP_DOUBLE);
}

Test(rw_json_storage, rw_array_get_bool)
{
    rw_array_get_type("[true]", T_BOOL);
}

Test(rw_json_storage, rw_array_get_null)
{
    rw_array_get_type("[null]", T_NULL);
}

Test(rw_json_storage, rw_array_get_emptyarray)
{
    rw_array_get_type("[[]]", T_ARR);
}

Test(rw_json_storage, rw_array_get_array)
{
    rw_array_get_type("[[1,2,3]]", T_ARR);
}

Test(rw_json_storage, rw_array_get_emptydict)
{
    rw_array_get_type("[{}]", T_DICT);
}

Test(rw_json_storage, rw_array_get_dict)
{
    rw_array_get_type("[{\"key\":1}]", T_DICT);
}

void rw_dict_get_type(char *buff, u8 expected_type)
{
    size_t idx   = 0;
    rw_dict_t *d = rw_parse_dict(buff, &idx);
    if (!d)
    {
        return;
    }

    rw_item_t *it = rw_dict_get(d, string_nofree_of("k"));
    if (!it)
    {
        destroy_rw_dict(d);
        return;
    }

    cr_expect(
        it->type == expected_type,
        "Expected the item to be of type %s (got '%s')",
        rw_get_type_as_str(expected_type), rw_get_type_as_str(it->type)
    );

    destroy_rw_dict(d);
}

Test(rw_json_storage, ro_dict_get_str)
{
    rw_dict_get_type("{\"k\":\"test\"}", T_STR);
}

Test(rw_json_storage, ro_dict_get_long)
{
    rw_dict_get_type("{\"k\":1}", T_LONG);
}

Test(rw_json_storage, ro_dict_get_double)
{
    rw_dict_get_type("{\"k\":1.5}", T_DOUBLE);
}

Test(rw_json_storage, ro_dict_get_explong)
{
    rw_dict_get_type("{\"k\":1e5}", T_EXP_LONG);
}

Test(rw_json_storage, ro_dict_get_expdouble)
{
    rw_dict_get_type("{\"k\":1.5e89}", T_EXP_DOUBLE);
}

Test(rw_json_storage, ro_dict_get_bool)
{
    rw_dict_get_type("{\"k\":true}", T_BOOL);
}

Test(rw_json_storage, ro_dict_get_null)
{
    rw_dict_get_type("{\"k\":null}", T_NULL);
}

Test(rw_json_storage, ro_dict_get_emptyarray)
{
    rw_dict_get_type("{\"k\":[]}", T_ARR);
}

Test(rw_json_storage, ro_dict_get_array)
{
    rw_dict_get_type("{\"k\":[1,2,3]}", T_ARR);
}

Test(rw_json_storage, ro_dict_get_emptydict)
{
    rw_dict_get_type("{\"k\":{}}", T_DICT);
}

Test(rw_json_storage, ro_dict_get_dict)
{
    rw_dict_get_type("{\"k\":{\"key\":1}}", T_DICT);
}
