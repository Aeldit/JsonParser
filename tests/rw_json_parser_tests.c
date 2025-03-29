#include <criterion/criterion.h>
#include <stdio.h>
#include <sys/stat.h>

#include "../src/base_json_parser.h"
#include "../src/base_json_storage.h"
#include "../src/rw_json_equality.h"
#include "../src/rw_json_parser.h"

#define JSON_TESTS_FILE ("tests/test.json")

/*******************************************************************************
**                              rw_PARSE_ARRAY_BUFF                           **
*******************************************************************************/
rw_array_t get_rw_array_from_file(size_t *idx)
{
    if (!idx)
    {
        return EMPTY_RW_ARRAY;
    }

    FILE *f = fopen(JSON_TESTS_FILE, "r");
    if (!f)
    {
        return EMPTY_RW_ARRAY;
    }

    rw_array_t a = EMPTY_RW_ARRAY;
    char *buff   = 0;

    if (fseek(f, 0, SEEK_SET) != 0)
    {
        fclose(f);
        return EMPTY_RW_ARRAY;
    }

    struct stat st;
    stat(JSON_TESTS_FILE, &st);
    size_t nb_chars = st.st_size;

    buff = calloc(nb_chars + 1, sizeof(char));
    if (!buff)
    {
        fclose(f);
        return EMPTY_RW_ARRAY;
    }
    fread(buff, sizeof(char), nb_chars, f);

    a = rw_parse_array(buff, idx);
    if (buff)
    {
        free(buff);
    }
    fclose(f);
    return a;
}

Test(rw_json_parser, rw_parse_array_buff_empty)
{
    size_t idx   = 518;
    rw_array_t a = get_rw_array_from_file(&idx);

    rw_array_t b = EMPTY_RW_ARRAY;

    cr_expect(
        rw_arrays_equal(a, b),
        "Expected the 2 arrays to be equal, but they were not"
    );

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_json_parser, rw_parse_array_buff_long_numbers)
{
    size_t idx   = 85;
    rw_array_t a = get_rw_array_from_file(&idx);

    rw_array_t b = init_rw_array_with(
        20, RWVAL_LONG(64220), RWVAL_LONG(-512), RWVAL_DOUBLE(642.25),
        RWVAL_DOUBLE(-642.25), RWVAL_EXPLONG_T(2, 8), RWVAL_EXPLONG_T(-53, 4),
        RWVAL_EXPLONG_T(200, -10), RWVAL_EXPLONG_T(-251, -10),
        RWVAL_EXPDOUBLE_T(200.5, 10), RWVAL_EXPDOUBLE_T(-200.5, 10),
        RWVAL_EXPDOUBLE_T(200.5, -10), RWVAL_EXPDOUBLE_T(-200.5, -10),
        RWVAL_EXPLONG_T(2, 8), RWVAL_EXPLONG_T(-53, 4),
        RWVAL_EXPLONG_T(200, -10), RWVAL_EXPLONG_T(-251, -10),
        RWVAL_EXPDOUBLE_T(200.5, 10), RWVAL_EXPDOUBLE_T(-200.5, 10),
        RWVAL_EXPDOUBLE_T(200.5, -10), RWVAL_EXPDOUBLE_T(-200.5, -10)
    );

    cr_expect(
        rw_arrays_equal(a, b),
        "Expected the 2 arrays to be equal, but they were not"
    );

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_json_parser, rw_parse_array_buff_nested_multi_type)
{
    size_t idx   = 508;
    rw_array_t a = get_rw_array_from_file(&idx);

    rw_array_t b = init_rw_array_with(
        8, RWVAL_ARR(EMPTY_RW_ARRAY), RWVAL_LONG(1), RWVAL_LONG(2),
        RWVAL_LONG(3),
        RWVAL_ARR(init_rw_array_with(
            4, RWVAL_STR(string_nofree_of("")), RWVAL_LONG(5),
            RWVAL_ARR(init_rw_array_with(
                3, RWVAL_STR(string_nofree_of("dd")), RWVAL_BOOL(true),
                RWVAL_LONG(5)
            )),
            RWVAL_DICT(EMPTY_RW_DICT)
        )),
        RWVAL_DICT(EMPTY_RW_DICT),
        RWVAL_DICT(init_rw_dict_with(
            1, RWIT_STR(string_nofree_of("first"), string_nofree_of("w"))
        )),
        RWVAL_DICT(EMPTY_RW_DICT)
    );

    cr_expect(
        rw_arrays_equal(a, b),
        "Expected the 2 arrays to be equal, but they were not"
    );

    destroy_rw_array(a);
    destroy_rw_array(b);
}

/*******************************************************************************
**                               rw_PARSE_DICT_BUFF                           **
*******************************************************************************/
rw_dict_t get_rw_dict_from_file(size_t *idx)
{
    if (!idx)
    {
        return EMPTY_RW_DICT;
    }

    FILE *f = fopen(JSON_TESTS_FILE, "r");
    if (!f)
    {
        return EMPTY_RW_DICT;
    }

    rw_dict_t a = EMPTY_RW_DICT;
    char *buff  = 0;

    if (fseek(f, 0, SEEK_SET) != 0)
    {
        fclose(f);
        return EMPTY_RW_DICT;
    }

    struct stat st;
    stat(JSON_TESTS_FILE, &st);
    size_t nb_chars = st.st_size;

    buff = calloc(nb_chars + 1, sizeof(char));
    if (!buff)
    {
        fclose(f);
        return EMPTY_RW_DICT;
    }
    fread(buff, sizeof(char), nb_chars, f);

    a = rw_parse_dict(buff, idx);

    if (buff)
    {
        free(buff);
    }
    fclose(f);
    return a;
}

Test(rw_json_parser, rw_parse_dict_buff_empty)
{
    size_t idx  = 812;
    rw_dict_t a = get_rw_dict_from_file(&idx);

    rw_dict_t b = EMPTY_RW_DICT;

    cr_expect(
        rw_dicts_equal(a, b),
        "Expected the 2 dicts to be equal, but they were not"
    );

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_json_parser, rw_parse_dict_buff_numbers)
{
    size_t idx  = 824;
    rw_dict_t a = get_rw_dict_from_file(&idx);

    rw_dict_t b = init_rw_dict_with(
        3, RWIT_LONG(string_nofree_of("1"), 1),
        RWIT_LONG(string_nofree_of("2"), 2), RWIT_LONG(string_nofree_of("3"), 3)
    );

    cr_expect(
        rw_dicts_equal(a, b),
        "Expected the 2 dicts to be equal, but they were not"
    );

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_json_parser, rw_parse_dict_buff_nested_multi_type)
{
    size_t idx  = 905;
    rw_dict_t a = get_rw_dict_from_file(&idx);

    rw_dict_t b = init_rw_dict_with(
        1,
        RWIT_DICT(
            string_nofree_of("array"),
            init_rw_dict_with(
                3, RWIT_LONG(string_nofree_of(""), 1),
                RWIT_ARR(
                    string_nofree_of("arr"),
                    init_rw_array_with(
                        3, RWVAL_STR(string_nofree_of("dd")), RWVAL_BOOL(true),
                        RWVAL_LONG(5)
                    )
                ),
                RWIT_DICT(string_nofree_of("d"), EMPTY_RW_DICT)
            )
        )
    );

    cr_expect(
        rw_dicts_equal(a, b),
        "Expected the 2 dicts to be equal, but they were not"
    );

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

/*******************************************************************************
**                                   rw_PARSE                                 **
*******************************************************************************/
Test(rw_json_parser, rw_parse_array)
{
    rw_json_t rw_json = rw_parse(JSON_TESTS_FILE);

    rw_array_t strings = init_rw_array_with(
        2, RWVAL_STR(string_nofree_of("testing normal string")),
        RWVAL_STR(string_nofree_of(""))
    );

    rw_array_t numbers = init_rw_array_with(
        20, RWVAL_LONG(64220), RWVAL_LONG(-512), RWVAL_DOUBLE(642.25),
        RWVAL_DOUBLE(-642.25), RWVAL_EXPLONG_T(2, 8), RWVAL_EXPLONG_T(-53, 4),
        RWVAL_EXPLONG_T(200, -10), RWVAL_EXPLONG_T(-251, -10),
        RWVAL_EXPDOUBLE_T(200.5, 10), RWVAL_EXPDOUBLE_T(-200.5, 10),
        RWVAL_EXPDOUBLE_T(200.5, -10), RWVAL_EXPDOUBLE_T(-200.5, -10),
        RWVAL_EXPLONG_T(2, 8), RWVAL_EXPLONG_T(-53, 4),
        RWVAL_EXPLONG_T(200, -10), RWVAL_EXPLONG_T(-251, -10),
        RWVAL_EXPDOUBLE_T(200.5, 10), RWVAL_EXPDOUBLE_T(-200.5, 10),
        RWVAL_EXPDOUBLE_T(200.5, -10), RWVAL_EXPDOUBLE_T(-200.5, -10)
    );

    rw_array_t booleans =
        init_rw_array_with(2, RWVAL_BOOL(true), RWVAL_BOOL(false));

    rw_array_t arrays = init_rw_array_with(
        8, RWVAL_ARR(EMPTY_RW_ARRAY), RWVAL_LONG(1), RWVAL_LONG(2),
        RWVAL_LONG(3),
        RWVAL_ARR(init_rw_array_with(
            4, RWVAL_STR(string_nofree_of("")), RWVAL_LONG(5),
            RWVAL_ARR(init_rw_array_with(
                3, RWVAL_STR(string_nofree_of("dd")), RWVAL_BOOL(true),
                RWVAL_LONG(5)
            )),
            RWVAL_DICT(EMPTY_RW_DICT)
        )),
        RWVAL_DICT(EMPTY_RW_DICT),
        RWVAL_DICT(init_rw_dict_with(
            1, RWIT_STR(string_nofree_of("first"), string_nofree_of("w"))
        )),
        RWVAL_DICT(EMPTY_RW_DICT)
    );

    rw_array_t dicts = init_rw_array_with(
        3, RWVAL_DICT(EMPTY_RW_DICT),
        RWVAL_DICT(init_rw_dict_with(
            3, RWIT_LONG(string_nofree_of("1"), 1),
            RWIT_LONG(string_nofree_of("2"), 2),
            RWIT_LONG(string_nofree_of("3"), 3)
        )),
        RWVAL_DICT(init_rw_dict_with(
            1,
            RWIT_DICT(
                string_nofree_of("array"),
                init_rw_dict_with(
                    3, RWIT_LONG(string_nofree_of(""), 1),
                    RWIT_ARR(
                        string_nofree_of("arr"),
                        init_rw_array_with(
                            3, RWVAL_STR(string_nofree_of("dd")),
                            RWVAL_BOOL(true), RWVAL_LONG(5)
                        )
                    ),
                    RWIT_DICT(string_nofree_of("d"), EMPTY_RW_DICT)
                )
            )
        ))
    );

    rw_dict_t file_dict = init_rw_dict_with(
        5, RWIT_ARR(string_nofree_of("strings"), strings),
        RWIT_ARR(string_nofree_of("numbers"), numbers),
        RWIT_ARR(string_nofree_of("booleans"), booleans),
        RWIT_ARR(string_nofree_of("arrays"), arrays),
        RWIT_ARR(string_nofree_of("dicts"), dicts)
    );

    rw_json_t rw_json_manual = RW_JSON(false, EMPTY_RW_ARRAY, file_dict);

    cr_expect(
        rw_json_equal(rw_json, rw_json_manual),
        "Expected the 2 dicts to be equal, but they were not"
    );

    destroy_rw_json(rw_json);
    destroy_rw_json(rw_json_manual);
}
