#include <criterion/criterion.h>

#include "../src/base_json_parser.h"
#include "../src/ro_json_parser.h"
#include "../src/ro_json_storage.h"

#define JSON_TESTS_FILE ("tests/test.json")

/*******************************************************************************
**                                 INIT_RO_ARRAY                              **
*******************************************************************************/
Test(ro_json_storage, init_ro_array_zero_size)
{
    unsigned size = 0;
    ro_array_t *a = init_ro_array(size);

    cr_expect(a->size == size, "Expected the array size to be %u, but got %u",
              size, a->size);
    cr_expect(!a->insert_index, "Expected the array size to be %u, but got %u",
              0, a->insert_index);
    cr_expect(!a->values,
              "Expected a->values to be a null pointer, but got '%p'",
              a->values);

    destroy_ro_array(a);
}

Test(ro_json_storage, init_ro_array_normal_size)
{
    unsigned size = 5;
    ro_array_t *a = init_ro_array(size);

    cr_expect(a->size == size, "Expected the array size to be %u, but got %u",
              size, a->size);
    cr_expect(!a->insert_index, "Expected the array size to be %u, but got %u",
              0, a->insert_index);
    cr_expect(a->values,
              "Expected a->values to not be a null pointer, but it was");

    destroy_ro_array(a);
}

/*******************************************************************************
**                                 INIT_RO_DICT                               **
*******************************************************************************/
Test(ro_json_storage, init_ro_dict_zero_size)
{
    unsigned size = 0;
    ro_dict_t *d = init_ro_dict(size);

    cr_expect(d->size == size, "Expected the dict size to be %u, but got %u",
              size, d->size);
    cr_expect(!d->insert_index,
              "Expected the dict 'insert_index' to be %u, but got %u", 0,
              d->insert_index);
    cr_expect(!d->items, "Expected d->items to be a null pointer, but got '%p'",
              d->items);

    destroy_ro_dict(d);
}

Test(ro_json_storage, init_ro_dict_normal_size)
{
    unsigned size = 5;
    ro_dict_t *d = init_ro_dict(size);

    cr_expect(d->size == size, "Expected the dict size to be %u, but got %u",
              size, d->size);
    cr_expect(!d->insert_index,
              "Expected the dict 'insert_index' to be %u, but got %u", 0,
              d->insert_index);
    cr_expect(d->items,
              "Expected d->items to not be a null pointer, but it was");

    destroy_ro_dict(d);
}

/*******************************************************************************
**                                 INIT_RO_JSON                               **
*******************************************************************************/
Test(ro_json_storage, init_ro_json_array_null)
{
    ro_json_t *j = init_ro_json(1, 0, 0);

    cr_expect(
        !j,
        "Expected init_ro_json(1, 0, 0) to return a null pointer but got '%p'",
        j);

    destroy_ro_json(j);
}

Test(ro_json_storage, init_ro_json_dict_null)
{
    ro_json_t *j = init_ro_json(0, 0, 0);

    cr_expect(
        !j,
        "Expected init_ro_json(0, 0, 0) to return a null pointer but got '%p'",
        j);

    destroy_ro_json(j);
}

Test(ro_json_storage, init_ro_json_array_nonnull)
{
    ro_array_t *a = init_ro_array(5);
    if (!a)
    {
        return;
    }

    ro_json_t *j = init_ro_json(1, a, 0);

    cr_expect(j,
              "Expected init_ro_json(1, a, 0) to return a valid "
              "pointer but got '%p'",
              j);
    if (!j)
    {
        destroy_ro_array(a);
        return;
    }

    cr_expect(
        j->array,
        "Expected j->array to be a valid pointer, but it was a null pointer");

    destroy_ro_json(j);
}

Test(ro_json_storage, init_ro_json_dict_nonnull)
{
    ro_dict_t *d = init_ro_dict(5);
    if (!d)
    {
        return;
    }

    ro_json_t *j = init_ro_json(0, 0, d);

    cr_expect(j,
              "Expected init_ro_json(0, 0, d) to return a valid pointer but it "
              "returned a null pointer");

    if (!j)
    {
        destroy_ro_dict(d);
    }

    cr_expect(
        j->dict,
        "Expected j->dict to be a valid pointer, but it was a null pointer");

    destroy_ro_json(j);
}

/*******************************************************************************
**                                  ARRAY ADDS                                **
*******************************************************************************/
#define RO_ARRAY_ADD_TYPE_TEST_NULL_VALUES(ro_array_add_type, value)           \
    ro_array_t *a = init_ro_array(0);                                          \
    ro_array_add_type(a, value);                                               \
    cr_expect(                                                                 \
        !a->insert_index,                                                      \
        "Expected the insert_index of the array to be 0 after the addition, "  \
        "because the values field of the array is null (got '%u')",            \
        a->insert_index);                                                      \
    destroy_ro_array(a)

#define RO_ARRAY_ADD_TYPE_TEST_ADD_3_TYPE(ro_array_add_type, v1, v2, v3)       \
    unsigned len = 3;                                                          \
    ro_array_t *a = init_ro_array(len);                                        \
    ro_array_add_type(a, v1);                                                  \
    ro_array_add_type(a, v2);                                                  \
    ro_array_add_type(a, v3);                                                  \
    cr_expect(a->insert_index == len,                                          \
              "Expected the insert_index of the array to be %u after the "     \
              "additions, but got '%u'",                                       \
              len, a->insert_index);                                           \
    destroy_ro_array(a)

Test(ro_json_storage, ro_array_add_str_array_null_values)
{
    RO_ARRAY_ADD_TYPE_TEST_NULL_VALUES(ro_array_add_str,
                                       string_nofree_of("test"));
}

Test(ro_json_storage, ro_array_add_str_array_add_3)
{
    RO_ARRAY_ADD_TYPE_TEST_ADD_3_TYPE(ro_array_add_str, string_nofree_of("t1"),
                                      string_nofree_of("t3"),
                                      string_nofree_of("t3"));
}

Test(ro_json_storage, ro_array_add_long_array_null_values)
{
    RO_ARRAY_ADD_TYPE_TEST_NULL_VALUES(ro_array_add_long, 123);
}

Test(ro_json_storage, ro_array_add_long_array_add_3)
{
    RO_ARRAY_ADD_TYPE_TEST_ADD_3_TYPE(ro_array_add_long, 1, 2, 3);
}

Test(ro_json_storage, ro_array_add_double_array_null_values)
{
    RO_ARRAY_ADD_TYPE_TEST_NULL_VALUES(ro_array_add_double, 1.5);
}

Test(ro_json_storage, ro_array_add_double_array_add_3)
{
    RO_ARRAY_ADD_TYPE_TEST_ADD_3_TYPE(ro_array_add_double, 1.5, 2.6, 3.7);
}

Test(ro_json_storage, ro_array_add_explong_array_null_values)
{
    RO_ARRAY_ADD_TYPE_TEST_NULL_VALUES(ro_array_add_exp_long,
                                       EXP_LONG_OF(2, 4));
}

Test(ro_json_storage, ro_array_add_explong_array_add_3)
{
    RO_ARRAY_ADD_TYPE_TEST_ADD_3_TYPE(ro_array_add_exp_long, EXP_LONG_OF(1, 2),
                                      EXP_LONG_OF(3, 4), EXP_LONG_OF(5, 6));
}

Test(ro_json_storage, ro_array_add_expdouble_array_null_values)
{
    RO_ARRAY_ADD_TYPE_TEST_NULL_VALUES(ro_array_add_exp_double,
                                       EXP_DOUBLE_OF(1.5, 2));
}

Test(ro_json_storage, ro_array_add_expdouble_array_add_3)
{
    RO_ARRAY_ADD_TYPE_TEST_ADD_3_TYPE(
        ro_array_add_exp_double, EXP_DOUBLE_OF(1.5, 2),
        EXP_DOUBLE_OF(2.5005, 3), EXP_DOUBLE_OF(0.555589, 5));
}

Test(ro_json_storage, ro_array_add_bool_array_null_values)
{
    RO_ARRAY_ADD_TYPE_TEST_NULL_VALUES(ro_array_add_bool, 1);
}

Test(ro_json_storage, ro_array_add_bool_array_add_3)
{
    RO_ARRAY_ADD_TYPE_TEST_ADD_3_TYPE(ro_array_add_bool, 0, 1, 0);
}

Test(ro_json_storage, ro_array_add_null_array_null_values)
{
    ro_array_t *a = init_ro_array(0);
    ro_array_add_null(a);
    cr_expect(
        !a->insert_index,
        "Expected the insert_index of the array to be 0 after the addition, "
        "because the values field of the array is null (got '%u')",
        a->insert_index);
    destroy_ro_array(a);
}

Test(ro_json_storage, ro_array_add_null_array_add_3)
{
    unsigned len = 3;
    ro_array_t *a = init_ro_array(len);
    ro_array_add_null(a);
    ro_array_add_null(a);
    ro_array_add_null(a);
    cr_expect(a->insert_index == len,
              "Expected the insert_index of the array to be %u after the "
              "additions, "
              "but got '%u'",
              len, a->insert_index);
    destroy_ro_array(a);
}

/*******************************************************************************
**                                  DICT ADDS                                 **
*******************************************************************************/
#define RO_DICT_ADD_TYPE_TEST_NULL_ITEMS(ro_dict_add_type, key, value)         \
    ro_dict_t *d = init_ro_dict(0);                                            \
    ro_dict_add_type(d, key, value);                                           \
    cr_expect(                                                                 \
        !d->insert_index,                                                      \
        "Expected the insert_index of the dict to be 0 after the addition, "   \
        "because the items field of the dict is null (got '%u')",              \
        d->insert_index);                                                      \
    destroy_ro_dict(d)

#define RO_DICT_ADD_TYPE_TEST_ADD_3_TYPE(ro_dict_add_type, k1, k2, k3, v1, v2, \
                                         v3)                                   \
    unsigned len = 3;                                                          \
    ro_dict_t *d = init_ro_dict(len);                                          \
    ro_dict_add_type(d, k1, v1);                                               \
    ro_dict_add_type(d, k2, v2);                                               \
    ro_dict_add_type(d, k3, v3);                                               \
    cr_expect(d->insert_index == len,                                          \
              "Expected the insert_index of the dict to be %u after the "      \
              "additions, but got '%u'",                                       \
              len, d->insert_index);                                           \
    destroy_ro_dict(d)

Test(ro_json_storage, ro_dict_add_str_array_null_items)
{
    RO_DICT_ADD_TYPE_TEST_NULL_ITEMS(ro_dict_add_str, string_nofree_of("key"),
                                     string_nofree_of("test"));
}

Test(ro_json_storage, ro_dict_add_str_array_add_3)
{
    RO_DICT_ADD_TYPE_TEST_ADD_3_TYPE(
        ro_dict_add_str, string_nofree_of("k1"), string_nofree_of("k2"),
        string_nofree_of("k3"), string_nofree_of("t1"), string_nofree_of("t2"),
        string_nofree_of("t3"));
}

Test(ro_json_storage, ro_dict_add_long_array_null_items)
{
    RO_DICT_ADD_TYPE_TEST_NULL_ITEMS(ro_dict_add_long, string_nofree_of("key"),
                                     5);
}

Test(ro_json_storage, ro_dict_add_long_array_add_3)
{
    RO_DICT_ADD_TYPE_TEST_ADD_3_TYPE(ro_dict_add_long, string_nofree_of("k1"),
                                     string_nofree_of("k2"),
                                     string_nofree_of("k3"), 1, 2, 45684898);
}

Test(ro_json_storage, ro_dict_add_double_array_null_items)
{
    RO_DICT_ADD_TYPE_TEST_NULL_ITEMS(ro_dict_add_double,
                                     string_nofree_of("key"), 5.52);
}

Test(ro_json_storage, ro_dict_add_double_array_add_3)
{
    RO_DICT_ADD_TYPE_TEST_ADD_3_TYPE(
        ro_dict_add_double, string_nofree_of("k1"), string_nofree_of("k2"),
        string_nofree_of("k3"), 0.1, 2.5468, 25.456848);
}

Test(ro_json_storage, ro_dict_add_explong_array_null_items)
{
    RO_DICT_ADD_TYPE_TEST_NULL_ITEMS(
        ro_dict_add_exp_long, string_nofree_of("key"), EXP_LONG_OF(5, 4));
}

Test(ro_json_storage, ro_dict_add_explong_array_add_3)
{
    RO_DICT_ADD_TYPE_TEST_ADD_3_TYPE(
        ro_dict_add_exp_long, string_nofree_of("k1"), string_nofree_of("k2"),
        string_nofree_of("k3"), EXP_LONG_OF(1, 2), EXP_LONG_OF(3, 4),
        EXP_LONG_OF(5, 6));
}

Test(ro_json_storage, ro_dict_add_expdouble_array_null_items)
{
    RO_DICT_ADD_TYPE_TEST_NULL_ITEMS(ro_dict_add_exp_double,
                                     string_nofree_of("key"),
                                     EXP_DOUBLE_OF(5.255, 3));
}

Test(ro_json_storage, ro_dict_add_expdouble_array_add_3)
{
    RO_DICT_ADD_TYPE_TEST_ADD_3_TYPE(
        ro_dict_add_exp_double, string_nofree_of("k1"), string_nofree_of("k2"),
        string_nofree_of("k3"), EXP_DOUBLE_OF(1.53, 45),
        EXP_DOUBLE_OF(-1.23, 4), EXP_DOUBLE_OF(0.002553, 5));
}

Test(ro_json_storage, ro_dict_add_bool_array_null_items)
{
    RO_DICT_ADD_TYPE_TEST_NULL_ITEMS(ro_dict_add_bool, string_nofree_of("key"),
                                     0);
}

Test(ro_json_storage, ro_dict_add_bool_array_add_3)
{
    RO_DICT_ADD_TYPE_TEST_ADD_3_TYPE(ro_dict_add_bool, string_nofree_of("k1"),
                                     string_nofree_of("k2"),
                                     string_nofree_of("k3"), 0, 1, 0);
}

Test(ro_json_storage, ro_dict_add_null_array_null_items)
{
    ro_dict_t *d = init_ro_dict(0);
    ro_dict_add_null(d, string_nofree_of("key"));
    cr_expect(
        !d->insert_index,
        "Expected the insert_index of the dict to be 0 after the addition, "
        "because the items field of the dict is null (got '%u')",
        d->insert_index);
    destroy_ro_dict(d);
}

Test(ro_json_storage, ro_dict_add_null_array_add_3)
{
    unsigned len = 3;
    ro_dict_t *d = init_ro_dict(len);
    ro_dict_add_null(d, string_nofree_of("k1"));
    ro_dict_add_null(d, string_nofree_of("k2"));
    ro_dict_add_null(d, string_nofree_of("k3"));
    cr_expect(d->insert_index == len,
              "Expected the insert_index of the dict to be %u after the "
              "additions, but got '%u'",
              len, d->insert_index);
    destroy_ro_dict(d);
}

/*******************************************************************************
**                                    GETS                                    **
*******************************************************************************/
Test(ro_json_storage, ro_array_get_null_array)
{
    unsigned i = 5;
    ro_value_t v = ro_array_get(0, i);

    cr_expect(v.type == T_ERROR,
              "Expected the value to be of type T_ERROR because we tried to "
              "get a value in a null array, but it wasn't");
}

Test(ro_json_storage, ro_array_get_empty)
{
    unsigned long idx = 0;
    char buff[] = "[]";
    ro_array_t *a = ro_parse_array_buff(buff, &idx);
    if (!a)
    {
        return;
    }

    unsigned i = 5;
    ro_value_t v = ro_array_get(a, i);

    cr_expect(
        v.type == T_ERROR,
        "Expected the value to be of type T_ERROR because we tried to "
        "get the value of index %u from an array of size 0, but it wasn't",
        i);

    destroy_ro_array(a);
}

Test(ro_json_storage, ro_dict_get_null_dict)
{
    ro_item_t it = ro_dict_get(0, string_nofree_of("test"));

    cr_expect(it.type == T_ERROR,
              "Expected the item to be of type T_ERROR because we tried to "
              "get an item from a null dict, but it wasn't");
}

Test(ro_json_storage, ro_dict_get_empty)
{
    unsigned long idx = 0;
    char buff[] = "{}";
    ro_dict_t *d = ro_parse_dict_buff(buff, &idx);
    if (!d)
    {
        return;
    }

    string_t key = string_nofree_of("test");
    ro_item_t it = ro_dict_get(d, key);

    cr_expect(it.type == T_ERROR,
              "Expected the value to be of type T_ERROR because we tried to "
              "get the value of key %s on a dict of size 0, but it wasn't",
              key.str);

    destroy_ro_dict(d);
}

char *ro_get_type_as_str(char type)
{
    switch (type)
    {
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

void ro_array_get_type(char *buff, char expected_type)
{
    unsigned long idx = 0;
    ro_array_t *a = ro_parse_array_buff(buff, &idx);
    if (!a)
    {
        return;
    }

    ro_value_t v = ro_array_get(a, 0);

    cr_expect(v.type == expected_type,
              "Expected the value to be of type %s (got '%s')",
              ro_get_type_as_str(expected_type), ro_get_type_as_str(v.type));

    destroy_ro_array(a);
}

Test(ro_json_storage, ro_array_get_str)
{
    ro_array_get_type("[\"test\"]", T_STR);
}

Test(ro_json_storage, ro_array_get_long)
{
    ro_array_get_type("[1]", T_LONG);
}

Test(ro_json_storage, ro_array_get_double)
{
    ro_array_get_type("[1.5]", T_DOUBLE);
}

Test(ro_json_storage, ro_array_get_explong)
{
    ro_array_get_type("[1e5]", T_EXP_LONG);
}

Test(ro_json_storage, ro_array_get_expdouble)
{
    ro_array_get_type("[1.5e89]", T_EXP_DOUBLE);
}

Test(ro_json_storage, ro_array_get_bool)
{
    ro_array_get_type("[true]", T_BOOL);
}

Test(ro_json_storage, ro_array_get_null)
{
    ro_array_get_type("[null]", T_NULL);
}

Test(ro_json_storage, ro_array_get_emptyarray)
{
    ro_array_get_type("[[]]", T_ARR);
}

Test(ro_json_storage, ro_array_get_array)
{
    ro_array_get_type("[[1,2,3]]", T_ARR);
}

Test(ro_json_storage, ro_array_get_emptydict)
{
    ro_array_get_type("[{}]", T_DICT);
}

Test(ro_json_storage, ro_array_get_dict)
{
    ro_array_get_type("[{\"key\":1}]", T_DICT);
}

void ro_dict_get_type(char *buff, char expected_type)
{
    unsigned long idx = 0;
    ro_dict_t *d = ro_parse_dict_buff(buff, &idx);
    if (!d)
    {
        return;
    }

    ro_item_t it = ro_dict_get(d, string_nofree_of("k"));

    cr_expect(it.type == expected_type,
              "Expected the item to be of type %s (got '%s')",
              ro_get_type_as_str(expected_type), ro_get_type_as_str(it.type));

    destroy_ro_dict(d);
}

Test(ro_json_storage, ro_dict_get_str)
{
    ro_dict_get_type("{\"k\":\"test\"}", T_STR);
}

Test(ro_json_storage, ro_dict_get_long)
{
    ro_dict_get_type("{\"k\":1}", T_LONG);
}

Test(ro_json_storage, ro_dict_get_double)
{
    ro_dict_get_type("{\"k\":1.5}", T_DOUBLE);
}

Test(ro_json_storage, ro_dict_get_explong)
{
    ro_dict_get_type("{\"k\":1e5}", T_EXP_LONG);
}

Test(ro_json_storage, ro_dict_get_expdouble)
{
    ro_dict_get_type("{\"k\":1.5e89}", T_EXP_DOUBLE);
}

Test(ro_json_storage, ro_dict_get_bool)
{
    ro_dict_get_type("{\"k\":true}", T_BOOL);
}

Test(ro_json_storage, ro_dict_get_null)
{
    ro_dict_get_type("{\"k\":null}", T_NULL);
}

Test(ro_json_storage, ro_dict_get_emptyarray)
{
    ro_dict_get_type("{\"k\":[]}", T_ARR);
}

Test(ro_json_storage, ro_dict_get_array)
{
    ro_dict_get_type("{\"k\":[1,2,3]}", T_ARR);
}

Test(ro_json_storage, ro_dict_get_emptydict)
{
    ro_dict_get_type("{\"k\":{}}", T_DICT);
}

Test(ro_json_storage, ro_dict_get_dict)
{
    ro_dict_get_type("{\"k\":{\"key\":1}}", T_DICT);
}
