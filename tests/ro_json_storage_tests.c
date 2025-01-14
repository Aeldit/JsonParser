#include <criterion/criterion.h>

#include "../src/base_json_parser.h"
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
**                                     ADDS                                   **
*******************************************************************************/
#define RO_ARRAY_ADD_STR_TEST_NULL_VALUES(ro_array_add_type, value)            \
    ro_array_t *a = init_ro_array(0);                                          \
    ro_array_add_type(a, value);                                               \
    cr_expect(                                                                 \
        !a->insert_index,                                                      \
        "Expected the insert_index of the array to be 0 after the addition, "  \
        "because the value field of the array is null (got '%u')",             \
        a->insert_index);                                                      \
    destroy_ro_array(a)

#define RO_ARRAY_ADD_STR_TEST_ADD_3_TYPE(ro_array_add_type, v1, v2, v3)        \
    unsigned len = 3;                                                          \
    ro_array_t *a = init_ro_array(len);                                        \
    ro_array_add_type(a, v1);                                                  \
    ro_array_add_type(a, v2);                                                  \
    ro_array_add_type(a, v3);                                                  \
    cr_expect(a->insert_index == len,                                          \
              "Expected the insert_index of the array to be %u after the "     \
              "additions, "                                                    \
              "but got '%u'",                                                  \
              len, a->insert_index);                                           \
    destroy_ro_array(a)

Test(ro_json_storage, ro_add_str_array_null_values)
{
    RO_ARRAY_ADD_STR_TEST_NULL_VALUES(ro_array_add_str,
                                      string_nofree_of("test"));
}

Test(ro_json_storage, ro_add_str_array_add_3)
{
    RO_ARRAY_ADD_STR_TEST_ADD_3_TYPE(ro_array_add_str, string_nofree_of("t1"),
                                     string_nofree_of("t3"),
                                     string_nofree_of("t3"));
}

Test(ro_json_storage, ro_add_long_array_null_values)
{
    RO_ARRAY_ADD_STR_TEST_NULL_VALUES(ro_array_add_long, 123);
}

Test(ro_json_storage, ro_add_long_array_add_3)
{
    RO_ARRAY_ADD_STR_TEST_ADD_3_TYPE(ro_array_add_long, 1, 2, 3);
}

Test(ro_json_storage, ro_add_double_array_null_values)
{
    RO_ARRAY_ADD_STR_TEST_NULL_VALUES(ro_array_add_double, 1.5);
}

Test(ro_json_storage, ro_add_double_array_add_3)
{
    RO_ARRAY_ADD_STR_TEST_ADD_3_TYPE(ro_array_add_double, 1.5, 2.6, 3.7);
}

Test(ro_json_storage, ro_add_explong_array_null_values)
{
    RO_ARRAY_ADD_STR_TEST_NULL_VALUES(ro_array_add_exp_long, EXP_LONG_OF(2, 4));
}

Test(ro_json_storage, ro_add_explong_array_add_3)
{
    RO_ARRAY_ADD_STR_TEST_ADD_3_TYPE(ro_array_add_exp_long, EXP_LONG_OF(1, 2),
                                     EXP_LONG_OF(3, 4), EXP_LONG_OF(5, 6));
}

Test(ro_json_storage, ro_add_expdouble_array_null_values)
{
    RO_ARRAY_ADD_STR_TEST_NULL_VALUES(ro_array_add_exp_double,
                                      EXP_DOUBLE_OF(1.5, 2));
}

Test(ro_json_storage, ro_add_expdouble_array_add_3)
{
    RO_ARRAY_ADD_STR_TEST_ADD_3_TYPE(
        ro_array_add_exp_double, EXP_DOUBLE_OF(1.5, 2),
        EXP_DOUBLE_OF(2.5005, 3), EXP_DOUBLE_OF(0.555589, 5));
}

Test(ro_json_storage, ro_add_bool_array_null_values)
{
    RO_ARRAY_ADD_STR_TEST_NULL_VALUES(ro_array_add_bool, 1);
}

Test(ro_json_storage, ro_add_bool_array_add_3)
{
    RO_ARRAY_ADD_STR_TEST_ADD_3_TYPE(ro_array_add_bool, 0, 1, 0);
}

Test(ro_json_storage, ro_add_null_array_null_values)
{
    ro_array_t *a = init_ro_array(0);
    ro_array_add_null(a);
    cr_expect(
        !a->insert_index,
        "Expected the insert_index of the array to be 0 after the addition, "
        "because the value field of the array is null (got '%u')",
        a->insert_index);
    destroy_ro_array(a);
}

Test(ro_json_storage, ro_add_null_array_add_3)
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
