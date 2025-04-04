#include <criterion/criterion.h>

#include "../src/ro_json_parser.h"
#include "../src/ro_json_storage.h"

#define JSON_TESTS_FILE ("tests/test.json")

/*******************************************************************************
**                                 RO_ARRAY                              **
*******************************************************************************/
Test(ro_json_storage, init_ro_array_zero_size)
{
    size_t size  = 0;
    ro_array_t a = RO_ARRAY(size);

    cr_expect(
        a.size == size, "Expected the array size to be %zu, but got %zu", size,
        a.size
    );
    cr_expect(
        !a.values, "Expected a->values to be a null pointer, but got '%p'",
        a.values
    );

    destroy_ro_array(a);
}

Test(ro_json_storage, init_ro_array_normal_size)
{
    size_t size  = 5;
    ro_array_t a = RO_ARRAY(size);

    cr_expect(
        a.size == size, "Expected the array size to be %zu, but got %zu", size,
        a.size
    );
    cr_expect(
        a.values, "Expected a->values to not be a null pointer, but it was"
    );

    destroy_ro_array(a);
}

/*******************************************************************************
**                                 RO_DICT                               **
*******************************************************************************/
Test(ro_json_storage, init_ro_dict_zero_size)
{
    size_t size = 0;
    ro_dict_t d = RO_DICT(size);

    cr_expect(
        d.size == size, "Expected the dict size to be %zu, but got %zu", size,
        d.size
    );
    cr_expect(
        !d.items, "Expected d->items to be a null pointer, but got '%p'",
        d.items
    );

    destroy_ro_dict(d);
}

Test(ro_json_storage, init_ro_dict_normal_size)
{
    size_t size = 5;
    ro_dict_t d = RO_DICT(size);

    cr_expect(
        d.size == size, "Expected the dict size to be %zu, but got %zu", size,
        d.size
    );
    cr_expect(
        d.items, "Expected d->items to not be a null pointer, but it was"
    );

    destroy_ro_dict(d);
}

/*******************************************************************************
**                                    GETS                                    **
*******************************************************************************/
Test(ro_json_storage, ro_array_get_empty)
{
    size_t idx   = 0;
    char buff[]  = "[]";
    ro_array_t a = ro_parse_array(buff, &idx);

    size_t i     = 5;
    ro_value_t v = ro_array_get(a, i);

    cr_expect(
        v.type == T_ERROR,
        "Expected the value to be of type T_ERROR because we tried to "
        "get the value of index %zu from an array of size 0, but it wasn't",
        i
    );

    destroy_ro_array(a);
}

Test(ro_json_storage, ro_dict_get_empty)
{
    size_t idx  = 0;
    char buff[] = "{}";
    ro_dict_t d = ro_parse_dict(buff, &idx);

    string_t key = string_nofree_of("test");
    ro_item_t it = ro_dict_get(d, key);

    cr_expect(
        it.type == T_ERROR,
        "Expected the value to be of type T_ERROR because we tried to "
        "get the value of key %s on a dict of size 0, but it wasn't",
        key.str
    );

    destroy_ro_dict(d);
}

char *ro_get_type_as_str(u8 type)
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

void ro_array_get_type(char *buff, u8 expected_type)
{
    size_t idx   = 0;
    ro_array_t a = ro_parse_array(buff, &idx);

    ro_value_t v = ro_array_get(a, 0);

    cr_expect(
        v.type == expected_type,
        "Expected the value to be of type %s (got '%s')",
        ro_get_type_as_str(expected_type), ro_get_type_as_str(v.type)
    );

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

void ro_dict_get_type(char *buff, u8 expected_type)
{
    size_t idx  = 0;
    ro_dict_t d = ro_parse_dict(buff, &idx);

    ro_item_t it = ro_dict_get(d, string_nofree_of("k"));

    cr_expect(
        it.type == expected_type,
        "Expected the item to be of type %s (got '%s')",
        ro_get_type_as_str(expected_type), ro_get_type_as_str(it.type)
    );

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
