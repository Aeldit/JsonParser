#include <criterion/criterion.h>
#include <stdio.h>
#include <sys/stat.h>

#include "../src/base_json_parser.h"
#include "../src/base_json_storage.h"
#include "../src/ro_json_equality.h"
#include "../src/ro_json_parser.h"
#include "../src/ro_json_storage.h"

#define JSON_TESTS_FILE ("tests/test.json")

/*******************************************************************************
**                              RO_PARSE_ARRAY_BUFF                           **
*******************************************************************************/
ro_array_t *get_ro_array_from_file(size_t *idx)
{
    if (!idx)
    {
        return 0;
    }

    FILE *f = fopen(JSON_TESTS_FILE, "r");
    if (!f)
    {
        return 0;
    }

    ro_array_t *a = 0;
    char *buff = 0;

    if (fseek(f, 0, SEEK_SET) != 0)
    {
        fclose(f);
        return 0;
    }

    struct stat st;
    stat(JSON_TESTS_FILE, &st);
    size_t nb_chars = st.st_size;

    buff = calloc(nb_chars + 1, sizeof(char));
    if (!buff)
    {
        fclose(f);
        return 0;
    }
    fread(buff, sizeof(char), nb_chars, f);

    a = ro_parse_array(buff, idx);
    free(buff);
    fclose(f);
    return a;
}

Test(ro_json_parser, ro_parse_array_buff_empty)
{
    size_t idx = 518;
    ro_array_t *a = get_ro_array_from_file(&idx);

    ro_array_t *b = init_ro_array(0);
    bool is_equal = ro_arrays_equal(a, b);

    cr_expect(is_equal, "Expected the 2 arrays to be equal, but they were not");
    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_json_parser, ro_parse_array_buff_long_numbers)
{
    size_t idx = 85;
    ro_array_t *a = get_ro_array_from_file(&idx);

    ro_array_t *b = init_ro_array(20);
    ro_value_t values[20] = {
        ROVAL_LONG(64220),
        ROVAL_LONG(-512),
        ROVAL_DOUBLE(642.25),
        ROVAL_DOUBLE(-642.25),
        ROVAL_EXPLONG_T(2, 8),
        ROVAL_EXPLONG_T(-53, 4),
        ROVAL_EXPLONG_T(200, -10),
        ROVAL_EXPLONG_T(-251, -10),
        ROVAL_EXPDOUBLE_T(200.5, 10),
        ROVAL_EXPDOUBLE_T(-200.5, 10),
        ROVAL_EXPDOUBLE_T(200.5, -10),
        ROVAL_EXPDOUBLE_T(-200.5, -10),
        ROVAL_EXPLONG_T(2, 8),
        ROVAL_EXPLONG_T(-53, 4),
        ROVAL_EXPLONG_T(200, -10),
        ROVAL_EXPLONG_T(-251, -10),
        ROVAL_EXPDOUBLE_T(200.5, 10),
        ROVAL_EXPDOUBLE_T(-200.5, 10),
        ROVAL_EXPDOUBLE_T(200.5, -10),
        ROVAL_EXPDOUBLE_T(-200.5, -10),
    };
    memcpy(b->values, values, 20 * sizeof(ro_value_t));

    bool is_equal = ro_arrays_equal(a, b);

    cr_expect(is_equal, "Expected the 2 arrays to be equal, but they were not");
    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_json_parser, ro_parse_array_buff_nested_multi_type)
{
    size_t idx = 508;
    ro_array_t *a = get_ro_array_from_file(&idx);

    ro_array_t *a3 = init_ro_array(3);
    ro_value_t a3_values[3] = {
        ROVAL_STR(string_nofree_of("dd")),
        ROVAL_BOOL(true),
        ROVAL_LONG(5),
    };
    memcpy(a3->values, a3_values, 3 * sizeof(ro_value_t));

    ro_array_t *a2 = init_ro_array(4);
    ro_value_t a2_values[4] = {
        ROVAL_STR(string_nofree_of("")),
        ROVAL_LONG(5),
        ROVAL_ARR(a3),
        ROVAL_DICT(init_ro_dict(0)),
    };
    memcpy(a2->values, a2_values, 4 * sizeof(ro_value_t));

    ro_dict_t *d = init_ro_dict(1);
    d->items[0] = ROIT_STR(string_nofree_of("first"), string_nofree_of("w"));

    ro_array_t *b = init_ro_array(8);
    ro_value_t values[8] = {
        ROVAL_ARR(init_ro_array(0)),
        ROVAL_LONG(1),
        ROVAL_LONG(2),
        ROVAL_LONG(3),
        ROVAL_ARR(a2),
        ROVAL_DICT(init_ro_dict(0)),
        ROVAL_DICT(d),
        ROVAL_DICT(init_ro_dict(0)),
    };
    memcpy(b->values, values, 8 * sizeof(ro_value_t));

    bool is_equal = ro_arrays_equal(a, b);

    cr_expect(is_equal, "Expected the 2 arrays to be equal, but they were not");
    destroy_ro_array(a);
    destroy_ro_array(b);
}

/*******************************************************************************
**                               RO_PARSE_DICT_BUFF                           **
*******************************************************************************/
ro_dict_t *get_ro_dict_from_file(size_t *idx)
{
    if (!idx)
    {
        return 0;
    }

    FILE *f = fopen(JSON_TESTS_FILE, "r");
    if (!f)
    {
        return 0;
    }

    ro_dict_t *a = 0;
    char *buff = 0;

    if (fseek(f, 0, SEEK_SET) != 0)
    {
        fclose(f);
        return 0;
    }

    struct stat st;
    stat(JSON_TESTS_FILE, &st);
    size_t nb_chars = st.st_size;

    buff = calloc(nb_chars + 1, sizeof(char));
    if (!buff)
    {
        fclose(f);
        return 0;
    }
    fread(buff, sizeof(char), nb_chars, f);

    a = ro_parse_dict(buff, idx);

    free(buff);
    fclose(f);
    return a;
}

Test(ro_json_parser, ro_parse_dict_buff_empty)
{
    size_t idx = 812;
    ro_dict_t *a = get_ro_dict_from_file(&idx);

    ro_dict_t *b = init_ro_dict(0);
    bool is_equal = ro_dicts_equal(a, b);

    cr_expect(is_equal, "Expected the 2 dicts to be equal, but they were not");
    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_json_parser, ro_parse_dict_buff_numbers)
{
    size_t idx = 824;
    ro_dict_t *a = get_ro_dict_from_file(&idx);

    ro_dict_t *b = init_ro_dict(3);
    b->items[0] = ROIT_LONG(string_nofree_of("1"), 1);
    b->items[1] = ROIT_LONG(string_nofree_of("2"), 2);
    b->items[2] = ROIT_LONG(string_nofree_of("3"), 3);

    bool is_equal = ro_dicts_equal(a, b);

    cr_expect(is_equal, "Expected the 2 dicts to be equal, but they were not");
    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_json_parser, ro_parse_dict_buff_nested_multi_type)
{
    size_t idx = 905;
    ro_dict_t *a = get_ro_dict_from_file(&idx);

    ro_array_t *a3 = init_ro_array(3);
    a3->values[0] = ROVAL_STR(string_nofree_of("dd"));
    a3->values[1] = ROVAL_BOOL(true);
    a3->values[2] = ROVAL_LONG(5);

    ro_dict_t *d2 = init_ro_dict(3);
    d2->items[0] = ROIT_LONG(string_nofree_of(""), 1);
    d2->items[1] = ROIT_ARR(string_nofree_of("arr"), a3);
    d2->items[2] = ROIT_DICT(string_nofree_of("d"), init_ro_dict(0));

    ro_dict_t *b = init_ro_dict(1);
    b->items[0] = ROIT_DICT(string_nofree_of("array"), d2);

    bool is_equal = ro_dicts_equal(a, b);

    cr_expect(is_equal, "Expected the 2 dicts to be equal, but they were not");
    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

/*******************************************************************************
**                                   RO_PARSE                                 **
*******************************************************************************/
Test(ro_json_parser, ro_parse_array)
{
    ro_json_t *ro_json = ro_parse(JSON_TESTS_FILE);
    if (!ro_json)
    {
        return;
    }

    ro_array_t *strings = init_ro_array_with(
        2, ROVAL_STR(string_nofree_of("testing normal string")),
        ROVAL_STR(string_nofree_of("")));

    ro_array_t *numbers = init_ro_array_with(
        20, ROVAL_LONG(64220), ROVAL_LONG(-512), ROVAL_DOUBLE(642.25),
        ROVAL_DOUBLE(-642.25), ROVAL_EXPLONG_T(2, 8), ROVAL_EXPLONG_T(-53, 4),
        ROVAL_EXPLONG_T(200, -10), ROVAL_EXPLONG_T(-251, -10),
        ROVAL_EXPDOUBLE_T(200.5, 10), ROVAL_EXPDOUBLE_T(-200.5, 10),
        ROVAL_EXPDOUBLE_T(200.5, -10), ROVAL_EXPDOUBLE_T(-200.5, -10),
        ROVAL_EXPLONG_T(2, 8), ROVAL_EXPLONG_T(-53, 4),
        ROVAL_EXPLONG_T(200, -10), ROVAL_EXPLONG_T(-251, -10),
        ROVAL_EXPDOUBLE_T(200.5, 10), ROVAL_EXPDOUBLE_T(-200.5, 10),
        ROVAL_EXPDOUBLE_T(200.5, -10), ROVAL_EXPDOUBLE_T(-200.5, -10));

    ro_array_t *booleans =
        init_ro_array_with(2, ROVAL_BOOL(true), ROVAL_BOOL(false));

    ro_array_t *arrays = init_ro_array_with(
        8, ROVAL_ARR(init_ro_array(0)), ROVAL_LONG(1), ROVAL_LONG(2),
        ROVAL_LONG(3),
        ROVAL_ARR(init_ro_array_with(
            4, ROVAL_STR(string_nofree_of("")), ROVAL_LONG(5),
            ROVAL_ARR(init_ro_array_with(3, ROVAL_STR(string_nofree_of("dd")),
                                         ROVAL_BOOL(true), ROVAL_LONG(5))),
            ROVAL_DICT(init_ro_dict(0)))),
        ROVAL_DICT(init_ro_dict(0)),
        ROVAL_DICT(init_ro_dict_with(
            1, ROIT_STR(string_nofree_of("first"), string_nofree_of("w")))),
        ROVAL_DICT(init_ro_dict(0)));

    ro_array_t *dicts = init_ro_array_with(
        3, ROVAL_DICT(init_ro_dict(0)),
        ROVAL_DICT(init_ro_dict_with(3, ROIT_LONG(string_nofree_of("1"), 1),
                                     ROIT_LONG(string_nofree_of("2"), 2),
                                     ROIT_LONG(string_nofree_of("3"), 3))),
        ROVAL_DICT(init_ro_dict_with(
            1,
            ROIT_DICT(
                string_nofree_of("array"),
                init_ro_dict_with(
                    3, ROIT_LONG(string_nofree_of(""), 1),
                    ROIT_ARR(
                        string_nofree_of("arr"),
                        init_ro_array_with(3, ROVAL_STR(string_nofree_of("dd")),
                                           ROVAL_BOOL(true), ROVAL_LONG(5))),
                    ROIT_DICT(string_nofree_of("d"), init_ro_dict(0)))))));

    ro_dict_t *file_dict =
        init_ro_dict_with(5, ROIT_ARR(string_nofree_of("strings"), strings),
                          ROIT_ARR(string_nofree_of("numbers"), numbers),
                          ROIT_ARR(string_nofree_of("booleans"), booleans),
                          ROIT_ARR(string_nofree_of("arrays"), arrays),
                          ROIT_ARR(string_nofree_of("dicts"), dicts));

    ro_json_t *ro_json_manual = init_ro_json(false, 0, file_dict);

    cr_expect(ro_json_equal(ro_json, ro_json_manual),
              "Expected the 2 dicts to be equal, but they were not");

    destroy_ro_json(ro_json);
    destroy_ro_json(ro_json_manual);
}
