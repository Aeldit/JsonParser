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
ro_array_t get_ro_array_from_file(size_t *idx)
{
    if (!idx)
    {
        return ERROR_RO_ARRAY;
    }

    FILE *f = fopen(JSON_TESTS_FILE, "r");
    if (!f)
    {
        return ERROR_RO_ARRAY;
    }

    ro_array_t a = ERROR_RO_ARRAY;
    char *buff   = 0;

    if (fseek(f, 0, SEEK_SET) != 0)
    {
        fclose(f);
        return ERROR_RO_ARRAY;
    }

    struct stat st;
    stat(JSON_TESTS_FILE, &st);
    size_t nb_chars = st.st_size;

    buff = calloc(nb_chars + 1, sizeof(char));
    if (!buff)
    {
        fclose(f);
        return ERROR_RO_ARRAY;
    }
    fread(buff, sizeof(char), nb_chars, f);
    fclose(f);

    a = ro_parse_array(buff, idx);
    free(buff);
    return a;
}

Test(ro_json_parser, ro_parse_array_buff_empty)
{
    size_t idx   = 518;
    ro_array_t a = get_ro_array_from_file(&idx);

    ro_array_t b = RO_ARRAY(0);

    cr_expect(
        ro_arrays_equal(a, b),
        "Expected the 2 arrays to be equal, but they were not"
    );

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_json_parser, ro_parse_array_buff_long_numbers)
{
    size_t idx   = 85;
    ro_array_t a = get_ro_array_from_file(&idx);

    ro_array_t b = init_ro_array_with(
        20, ROVAL_LONG(64220), ROVAL_LONG(-512), ROVAL_DOUBLE(642.25),
        ROVAL_DOUBLE(-642.25), ROVAL_EXPLONG_T(2, 8), ROVAL_EXPLONG_T(-53, 4),
        ROVAL_EXPLONG_T(200, -10), ROVAL_EXPLONG_T(-251, -10),
        ROVAL_EXPDOUBLE_T(200.5, 10), ROVAL_EXPDOUBLE_T(-200.5, 10),
        ROVAL_EXPDOUBLE_T(200.5, -10), ROVAL_EXPDOUBLE_T(-200.5, -10),
        ROVAL_EXPLONG_T(2, 8), ROVAL_EXPLONG_T(-53, 4),
        ROVAL_EXPLONG_T(200, -10), ROVAL_EXPLONG_T(-251, -10),
        ROVAL_EXPDOUBLE_T(200.5, 10), ROVAL_EXPDOUBLE_T(-200.5, 10),
        ROVAL_EXPDOUBLE_T(200.5, -10), ROVAL_EXPDOUBLE_T(-200.5, -10)
    );

    cr_expect(
        ro_arrays_equal(a, b),
        "Expected the 2 arrays to be equal, but they were not"
    );

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_json_parser, ro_parse_array_buff_nested_multi_type)
{
    size_t idx   = 508;
    ro_array_t a = get_ro_array_from_file(&idx);

    ro_array_t b = init_ro_array_with(
        8, ROVAL_ARR(RO_ARRAY(0)), ROVAL_LONG(1), ROVAL_LONG(2), ROVAL_LONG(3),
        ROVAL_ARR(init_ro_array_with(
            4, ROVAL_STR(string_nofree_of("")), ROVAL_LONG(5),
            ROVAL_ARR(init_ro_array_with(
                3, ROVAL_STR(string_nofree_of("dd")), ROVAL_BOOL(true),
                ROVAL_LONG(5)
            )),
            ROVAL_DICT(RO_DICT(0))
        )),
        ROVAL_DICT(RO_DICT(0)),
        ROVAL_DICT(init_ro_dict_with(
            1, ROIT_STR(string_nofree_of("first"), string_nofree_of("w"))
        )),
        ROVAL_DICT(RO_DICT(0))
    );
    printf("%zu\n", a.size);
    ro_array_print(a);
    ro_array_print(b);

    cr_expect(
        ro_arrays_equal(a, b),
        "Expected the 2 arrays to be equal, but they were not"
    );

    destroy_ro_array(a);
    destroy_ro_array(b);
}

/*******************************************************************************
**                               RO_PARSE_DICT_BUFF                           **
*******************************************************************************/
ro_dict_t get_ro_dict_from_file(size_t *idx)
{
    if (!idx)
    {
        return ERROR_RO_DICT;
    }

    FILE *f = fopen(JSON_TESTS_FILE, "r");
    if (!f)
    {
        return ERROR_RO_DICT;
    }

    ro_dict_t a = ERROR_RO_DICT;
    char *buff  = 0;

    if (fseek(f, 0, SEEK_SET) != 0)
    {
        fclose(f);
        return ERROR_RO_DICT;
    }

    struct stat st;
    stat(JSON_TESTS_FILE, &st);
    size_t nb_chars = st.st_size;

    buff = calloc(nb_chars + 1, sizeof(char));
    if (!buff)
    {
        fclose(f);
        return ERROR_RO_DICT;
    }
    fread(buff, sizeof(char), nb_chars, f);
    fclose(f);

    a = ro_parse_dict(buff, idx);
    free(buff);
    return a;
}

Test(ro_json_parser, ro_parse_dict_buff_empty)
{
    size_t idx  = 812;
    ro_dict_t d = get_ro_dict_from_file(&idx);

    ro_dict_t b   = RO_DICT(0);
    bool is_equal = ro_dicts_equal(d, b);

    cr_expect(is_equal, "Expected the 2 dicts to be equal, but they were not");
    destroy_ro_dict(d);
    destroy_ro_dict(b);
}

Test(ro_json_parser, ro_parse_dict_buff_numbers)
{
    size_t idx  = 824;
    ro_dict_t d = get_ro_dict_from_file(&idx);

    ro_dict_t b = init_ro_dict_with(
        3, ROIT_LONG(string_nofree_of("1"), 1),
        ROIT_LONG(string_nofree_of("2"), 2), ROIT_LONG(string_nofree_of("3"), 3)
    );

    bool is_equal = ro_dicts_equal(d, b);

    cr_expect(is_equal, "Expected the 2 dicts to be equal, but they were not");
    destroy_ro_dict(d);
    destroy_ro_dict(b);
}

Test(ro_json_parser, ro_parse_dict_buff_nested_multi_type)
{
    size_t idx  = 905;
    ro_dict_t d = get_ro_dict_from_file(&idx);

    ro_array_t a3 = init_ro_array_with(
        3, ROVAL_STR(string_nofree_of("dd")), ROVAL_BOOL(true), ROVAL_LONG(5)
    );

    ro_dict_t d2 = init_ro_dict_with(
        3, ROIT_LONG(string_nofree_of(""), 1),
        ROIT_ARR(string_nofree_of("arr"), a3),
        ROIT_DICT(string_nofree_of("d"), RO_DICT(0))
    );

    ro_dict_t b =
        init_ro_dict_with(1, ROIT_DICT(string_nofree_of("array"), d2));

    bool is_equal = ro_dicts_equal(d, b);

    cr_expect(is_equal, "Expected the 2 dicts to be equal, but they were not");
    destroy_ro_dict(d);
    destroy_ro_dict(b);
}

/*******************************************************************************
**                                   RO_PARSE                                 **
*******************************************************************************/
Test(ro_json_parser, ro_parse_array)
{
    ro_json_t ro_json = ro_parse(JSON_TESTS_FILE);

    ro_array_t strings = init_ro_array_with(
        2, ROVAL_STR(string_nofree_of("testing normal string")),
        ROVAL_STR(string_nofree_of(""))
    );

    ro_array_t numbers = init_ro_array_with(
        20, ROVAL_LONG(64220), ROVAL_LONG(-512), ROVAL_DOUBLE(642.25),
        ROVAL_DOUBLE(-642.25), ROVAL_EXPLONG_T(2, 8), ROVAL_EXPLONG_T(-53, 4),
        ROVAL_EXPLONG_T(200, -10), ROVAL_EXPLONG_T(-251, -10),
        ROVAL_EXPDOUBLE_T(200.5, 10), ROVAL_EXPDOUBLE_T(-200.5, 10),
        ROVAL_EXPDOUBLE_T(200.5, -10), ROVAL_EXPDOUBLE_T(-200.5, -10),
        ROVAL_EXPLONG_T(2, 8), ROVAL_EXPLONG_T(-53, 4),
        ROVAL_EXPLONG_T(200, -10), ROVAL_EXPLONG_T(-251, -10),
        ROVAL_EXPDOUBLE_T(200.5, 10), ROVAL_EXPDOUBLE_T(-200.5, 10),
        ROVAL_EXPDOUBLE_T(200.5, -10), ROVAL_EXPDOUBLE_T(-200.5, -10)
    );

    ro_array_t booleans =
        init_ro_array_with(2, ROVAL_BOOL(true), ROVAL_BOOL(false));

    ro_array_t arrays = init_ro_array_with(
        8, ROVAL_ARR(RO_ARRAY(0)), ROVAL_LONG(1), ROVAL_LONG(2), ROVAL_LONG(3),
        ROVAL_ARR(init_ro_array_with(
            4, ROVAL_STR(string_nofree_of("")), ROVAL_LONG(5),
            ROVAL_ARR(init_ro_array_with(
                3, ROVAL_STR(string_nofree_of("dd")), ROVAL_BOOL(true),
                ROVAL_LONG(5)
            )),
            ROVAL_DICT(RO_DICT(0))
        )),
        ROVAL_DICT(RO_DICT(0)),
        ROVAL_DICT(init_ro_dict_with(
            1, ROIT_STR(string_nofree_of("first"), string_nofree_of("w"))
        )),
        ROVAL_DICT(RO_DICT(0))
    );

    ro_array_t dicts = init_ro_array_with(
        3, ROVAL_DICT(RO_DICT(0)),
        ROVAL_DICT(init_ro_dict_with(
            3, ROIT_LONG(string_nofree_of("1"), 1),
            ROIT_LONG(string_nofree_of("2"), 2),
            ROIT_LONG(string_nofree_of("3"), 3)
        )),
        ROVAL_DICT(init_ro_dict_with(
            1,
            ROIT_DICT(
                string_nofree_of("array"),
                init_ro_dict_with(
                    3, ROIT_LONG(string_nofree_of(""), 1),
                    ROIT_ARR(
                        string_nofree_of("arr"),
                        init_ro_array_with(
                            3, ROVAL_STR(string_nofree_of("dd")),
                            ROVAL_BOOL(true), ROVAL_LONG(5)
                        )
                    ),
                    ROIT_DICT(string_nofree_of("d"), RO_DICT(0))
                )
            )
        ))
    );

    ro_dict_t file_dict = init_ro_dict_with(
        5, ROIT_ARR(string_nofree_of("strings"), strings),
        ROIT_ARR(string_nofree_of("numbers"), numbers),
        ROIT_ARR(string_nofree_of("booleans"), booleans),
        ROIT_ARR(string_nofree_of("arrays"), arrays),
        ROIT_ARR(string_nofree_of("dicts"), dicts)
    );

    ro_json_t ro_json_manual = RO_JSON(false, ERROR_RO_ARRAY, file_dict);

    cr_expect(
        ro_json_equal(ro_json, ro_json_manual),
        "Expected the 2 dicts to be equal, but they were not"
    );

    destroy_ro_json(ro_json);
    destroy_ro_json(ro_json_manual);
}
