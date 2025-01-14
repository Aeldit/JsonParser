#include <criterion/criterion.h>

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
              "Expected a->values to be a null pointer, but it pointed to '%p'",
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
    cr_expect(!d->items,
              "Expected d->items to be a null pointer, but it pointed to '%p'",
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
        "Expected j->didn to be a valid pointer, but it was a null pointer");

    destroy_ro_json(j);
}
