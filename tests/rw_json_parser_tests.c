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
rw_array_t *get_rw_array_from_fileile(size_t *idx, bool is_buff)
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

    rw_array_t *a = 0;
    char *buff = 0;

    if (is_buff)
    {
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

        a = rw_parse_array_buff(buff, idx);
    }
    else
    {
        if (fseek(f, *idx, SEEK_SET) != 0)
        {
            fclose(f);
            return 0;
        }
        a = rw_parse_array(f, idx);
    }

    if (buff)
    {
        free(buff);
    }
    fclose(f);
    return a;
}

Test(rw_json_parser, rw_parse_array_buff_empty)
{
    size_t idx = 518;
    rw_array_t *a = get_rw_array_from_fileile(&idx, true);

    rw_array_t *b = init_rw_array();
    bool is_equal = rw_arrays_equal(a, b);

    cr_expect(is_equal, "Expected the 2 arrays to be equal, but they were not");
    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_json_parser, rw_parse_array_buff_long_numbers)
{
    size_t idx = 85;
    rw_array_t *a = get_rw_array_from_fileile(&idx, true);

    rw_array_t *b = init_rw_array();

    rw_array_add_long(b, 64220);
    rw_array_add_long(b, -512);

    rw_array_add_double(b, 642.25);
    rw_array_add_double(b, -642.25);

    rw_array_add_exp_long(b, EXP_LONG_OF(2, 8));
    rw_array_add_exp_long(b, EXP_LONG_OF(-53, 4));
    rw_array_add_exp_long(b, EXP_LONG_OF(200, -10));
    rw_array_add_exp_long(b, EXP_LONG_OF(-251, -10));

    rw_array_add_exp_double(b, EXP_DOUBLE_OF(200.5, 10));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(-200.5, 10));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(200.5, -10));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(-200.5, -10));

    rw_array_add_exp_long(b, EXP_LONG_OF(2, 8));
    rw_array_add_exp_long(b, EXP_LONG_OF(-53, 4));
    rw_array_add_exp_long(b, EXP_LONG_OF(200, -10));
    rw_array_add_exp_long(b, EXP_LONG_OF(-251, -10));

    rw_array_add_exp_double(b, EXP_DOUBLE_OF(200.5, 10));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(-200.5, 10));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(200.5, -10));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(-200.5, -10));

    bool is_equal = rw_arrays_equal(a, b);

    cr_expect(is_equal, "Expected the 2 arrays to be equal, but they were not");
    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_json_parser, rw_parse_array_buff_nested_multi_type)
{
    size_t idx = 508;
    rw_array_t *a = get_rw_array_from_fileile(&idx, true);

    rw_array_t *b = init_rw_array();

    rw_array_add_array(b, init_rw_array());

    rw_array_add_long(b, 1);
    rw_array_add_long(b, 2);
    rw_array_add_long(b, 3);

    rw_array_t *a2 = init_rw_array();
    rw_array_add_str(a2, string_nofree_of(""));
    rw_array_add_long(a2, 5);

    rw_array_t *a3 = init_rw_array();
    rw_array_add_str(a3, string_nofree_of("dd"));
    rw_array_add_bool(a3, true);
    rw_array_add_long(a3, 5);

    rw_array_add_array(a2, a3);
    rw_array_add_dict(a2, init_rw_dict());

    rw_array_add_array(b, a2);
    rw_array_add_dict(b, init_rw_dict());

    rw_dict_t *d = init_rw_dict();
    rw_dict_add_str(d, string_nofree_of("first"), string_nofree_of("w"));

    rw_array_add_dict(b, d);
    rw_array_add_dict(b, init_rw_dict());

    bool is_equal = rw_arrays_equal(a, b);

    cr_expect(is_equal, "Expected the 2 arrays to be equal, but they were not");
    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_json_parser, rw_parse_array_empty)
{
    size_t idx = 519;
    rw_array_t *a = get_rw_array_from_fileile(&idx, false);

    rw_array_t *b = init_rw_array();
    bool is_equal = rw_arrays_equal(a, b);

    cr_expect(is_equal, "Expected the 2 arrays to be equal, but they were not");
    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_json_parser, rw_parse_array_long_numbers)
{
    size_t idx = 86;
    rw_array_t *a = get_rw_array_from_fileile(&idx, false);

    rw_array_t *b = init_rw_array();

    rw_array_add_long(b, 64220);
    rw_array_add_long(b, -512);

    rw_array_add_double(b, 642.25);
    rw_array_add_double(b, -642.25);

    rw_array_add_exp_long(b, EXP_LONG_OF(2, 8));
    rw_array_add_exp_long(b, EXP_LONG_OF(-53, 4));
    rw_array_add_exp_long(b, EXP_LONG_OF(200, -10));
    rw_array_add_exp_long(b, EXP_LONG_OF(-251, -10));

    rw_array_add_exp_double(b, EXP_DOUBLE_OF(200.5, 10));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(-200.5, 10));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(200.5, -10));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(-200.5, -10));

    rw_array_add_exp_long(b, EXP_LONG_OF(2, 8));
    rw_array_add_exp_long(b, EXP_LONG_OF(-53, 4));
    rw_array_add_exp_long(b, EXP_LONG_OF(200, -10));
    rw_array_add_exp_long(b, EXP_LONG_OF(-251, -10));

    rw_array_add_exp_double(b, EXP_DOUBLE_OF(200.5, 10));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(-200.5, 10));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(200.5, -10));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(-200.5, -10));

    bool is_equal = rw_arrays_equal(a, b);

    cr_expect(is_equal, "Expected the 2 arrays to be equal, but they were not");
    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_json_parser, rw_parse_array_nested_multi_type)
{
    size_t idx = 509;
    rw_array_t *a = get_rw_array_from_fileile(&idx, false);

    rw_array_t *b = init_rw_array();

    rw_array_add_array(b, init_rw_array());

    rw_array_add_long(b, 1);
    rw_array_add_long(b, 2);
    rw_array_add_long(b, 3);

    rw_array_t *a2 = init_rw_array();
    rw_array_add_str(a2, string_nofree_of(""));
    rw_array_add_long(a2, 5);

    rw_array_t *a3 = init_rw_array();
    rw_array_add_str(a3, string_nofree_of("dd"));
    rw_array_add_bool(a3, true);
    rw_array_add_long(a3, 5);

    rw_array_add_array(a2, a3);
    rw_array_add_dict(a2, init_rw_dict());

    rw_array_add_array(b, a2);
    rw_array_add_dict(b, init_rw_dict());

    rw_dict_t *d = init_rw_dict();
    rw_dict_add_str(d, string_nofree_of("first"), string_nofree_of("w"));

    rw_array_add_dict(b, d);
    rw_array_add_dict(b, init_rw_dict());

    bool is_equal = rw_arrays_equal(a, b);

    cr_expect(is_equal, "Expected the 2 arrays to be equal, but they were not");
    destroy_rw_array(a);
    destroy_rw_array(b);
}

/*******************************************************************************
**                               rw_PARSE_DICT_BUFF                           **
*******************************************************************************/
rw_dict_t *get_rw_dict_from_fileile(size_t *idx, bool is_buff)
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

    rw_dict_t *a = 0;
    char *buff = 0;

    if (is_buff)
    {
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

        a = rw_parse_dict_buff(buff, idx);
    }
    else
    {
        if (fseek(f, *idx, SEEK_SET) != 0)
        {
            fclose(f);
            return 0;
        }
        a = rw_parse_dict(f, idx);
    }

    if (buff)
    {
        free(buff);
    }
    fclose(f);
    return a;
}

Test(rw_json_parser, rw_parse_dict_buff_empty)
{
    size_t idx = 812;
    rw_dict_t *a = get_rw_dict_from_fileile(&idx, true);

    rw_dict_t *b = init_rw_dict();
    bool is_equal = rw_dicts_equal(a, b);

    cr_expect(is_equal, "Expected the 2 dicts to be equal, but they were not");
    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_json_parser, rw_parse_dict_buff_numbers)
{
    size_t idx = 824;
    rw_dict_t *a = get_rw_dict_from_fileile(&idx, true);

    rw_dict_t *b = init_rw_dict();

    rw_dict_add_long(b, string_nofree_of("1"), 1);
    rw_dict_add_long(b, string_nofree_of("2"), 2);
    rw_dict_add_long(b, string_nofree_of("3"), 3);

    bool is_equal = rw_dicts_equal(a, b);

    cr_expect(is_equal, "Expected the 2 dicts to be equal, but they were not");
    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_json_parser, rw_parse_dict_buff_nested_multi_type)
{
    size_t idx = 905;
    rw_dict_t *a = get_rw_dict_from_fileile(&idx, true);

    rw_array_t *a3 = init_rw_array();
    rw_array_add_str(a3, string_nofree_of("dd"));
    rw_array_add_bool(a3, true);
    rw_array_add_long(a3, 5);

    rw_dict_t *a2 = init_rw_dict();
    rw_dict_add_long(a2, string_nofree_of(""), 1);
    rw_dict_add_array(a2, string_nofree_of("arr"), a3);
    rw_dict_add_dict(a2, string_nofree_of("d"), init_rw_dict());

    rw_dict_t *b = init_rw_dict();
    rw_dict_add_dict(b, string_nofree_of("array"), a2);

    bool is_equal = rw_dicts_equal(a, b);

    cr_expect(is_equal, "Expected the 2 dicts to be equal, but they were not");
    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_json_parser, rw_parse_dict_empty)
{
    size_t idx = 813;
    rw_dict_t *a = get_rw_dict_from_fileile(&idx, false);

    rw_dict_t *b = init_rw_dict();
    bool is_equal = rw_dicts_equal(a, b);

    cr_expect(is_equal, "Expected the 2 dicts to be equal, but they were not");
    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_json_parser, rw_parse_dict_numbers)
{
    size_t idx = 825;
    rw_dict_t *a = get_rw_dict_from_fileile(&idx, false);

    rw_dict_t *b = init_rw_dict();

    rw_dict_add_long(b, string_nofree_of("1"), 1);
    rw_dict_add_long(b, string_nofree_of("2"), 2);
    rw_dict_add_long(b, string_nofree_of("3"), 3);

    bool is_equal = rw_dicts_equal(a, b);

    cr_expect(is_equal, "Expected the 2 dicts to be equal, but they were not");
    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_json_parser, rw_parse_dict_nested_multi_type)
{
    size_t idx = 906;
    rw_dict_t *a = get_rw_dict_from_fileile(&idx, false);

    rw_array_t *a3 = init_rw_array();
    rw_array_add_str(a3, string_nofree_of("dd"));
    rw_array_add_bool(a3, true);
    rw_array_add_long(a3, 5);

    rw_dict_t *a2 = init_rw_dict();
    rw_dict_add_long(a2, string_nofree_of(""), 1);
    rw_dict_add_array(a2, string_nofree_of("arr"), a3);
    rw_dict_add_dict(a2, string_nofree_of("d"), init_rw_dict());

    rw_dict_t *b = init_rw_dict();
    rw_dict_add_dict(b, string_nofree_of("array"), a2);

    bool is_equal = rw_dicts_equal(a, b);

    cr_expect(is_equal, "Expected the 2 dicts to be equal, but they were not");
    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

/*******************************************************************************
**                                   rw_PARSE                                 **
*******************************************************************************/
Test(rw_json_parser, rw_parse_array)
{
    rw_json_t *rw_json = rw_parse(JSON_TESTS_FILE);
    if (!rw_json)
    {
        return;
    }

    rw_array_t *strings = init_rw_array();
    rw_array_add_str(strings, string_nofree_of("testing normal string"));
    rw_array_add_str(strings, string_nofree_of(""));

    rw_array_t *numbers = init_rw_array();
    rw_array_add_long(numbers, 64220);
    rw_array_add_long(numbers, -512);
    rw_array_add_double(numbers, 642.25);
    rw_array_add_double(numbers, -642.25);
    rw_array_add_exp_long(numbers, EXP_LONG_OF(2, 8));
    rw_array_add_exp_long(numbers, EXP_LONG_OF(-53, 4));
    rw_array_add_exp_long(numbers, EXP_LONG_OF(200, -10));
    rw_array_add_exp_long(numbers, EXP_LONG_OF(-251, -10));
    rw_array_add_exp_double(numbers, EXP_DOUBLE_OF(200.5, 10));
    rw_array_add_exp_double(numbers, EXP_DOUBLE_OF(-200.5, 10));
    rw_array_add_exp_double(numbers, EXP_DOUBLE_OF(200.5, -10));
    rw_array_add_exp_double(numbers, EXP_DOUBLE_OF(-200.5, -10));
    rw_array_add_exp_long(numbers, EXP_LONG_OF(2, 8));
    rw_array_add_exp_long(numbers, EXP_LONG_OF(-53, 4));
    rw_array_add_exp_long(numbers, EXP_LONG_OF(200, -10));
    rw_array_add_exp_long(numbers, EXP_LONG_OF(-251, -10));
    rw_array_add_exp_double(numbers, EXP_DOUBLE_OF(200.5, 10));
    rw_array_add_exp_double(numbers, EXP_DOUBLE_OF(-200.5, 10));
    rw_array_add_exp_double(numbers, EXP_DOUBLE_OF(200.5, -10));
    rw_array_add_exp_double(numbers, EXP_DOUBLE_OF(-200.5, -10));

    rw_array_t *booleans = init_rw_array();
    rw_array_add_bool(booleans, true);
    rw_array_add_bool(booleans, false);

    rw_array_t *arrays = init_rw_array();
    rw_array_add_array(arrays, init_rw_array());
    rw_array_add_long(arrays, 1);
    rw_array_add_long(arrays, 2);
    rw_array_add_long(arrays, 3);
    rw_array_t *arrays_a2 = init_rw_array();
    rw_array_add_str(arrays_a2, string_nofree_of(""));
    rw_array_add_long(arrays_a2, 5);
    rw_array_t *arrays_a3 = init_rw_array();
    rw_array_add_str(arrays_a3, string_nofree_of("dd"));
    rw_array_add_bool(arrays_a3, true);
    rw_array_add_long(arrays_a3, 5);
    rw_array_add_array(arrays_a2, arrays_a3);
    rw_array_add_dict(arrays_a2, init_rw_dict());
    rw_array_add_array(arrays, arrays_a2);
    rw_array_add_dict(arrays, init_rw_dict());
    rw_dict_t *arrays_d = init_rw_dict();
    rw_dict_add_str(arrays_d, string_nofree_of("first"), string_nofree_of("w"));
    rw_array_add_dict(arrays, arrays_d);
    rw_array_add_dict(arrays, init_rw_dict());

    rw_array_t *dicts = init_rw_array();
    rw_array_add_dict(dicts, init_rw_dict());
    rw_dict_t *dicts_d1 = init_rw_dict();
    rw_dict_add_long(dicts_d1, string_nofree_of("1"), 1);
    rw_dict_add_long(dicts_d1, string_nofree_of("2"), 2);
    rw_dict_add_long(dicts_d1, string_nofree_of("3"), 3);
    rw_array_add_dict(dicts, dicts_d1);
    rw_dict_t *dicts_d2 = init_rw_dict();
    rw_dict_t *dicts_d2_array = init_rw_dict();
    rw_dict_add_long(dicts_d2_array, string_nofree_of(""), 1);
    rw_array_t *dicts_d2_array_arr = init_rw_array();
    rw_array_add_str(dicts_d2_array_arr, string_nofree_of("dd"));
    rw_array_add_bool(dicts_d2_array_arr, true);
    rw_array_add_long(dicts_d2_array_arr, 5);
    rw_dict_add_array(dicts_d2_array, string_nofree_of("arr"),
                      dicts_d2_array_arr);
    rw_dict_add_dict(dicts_d2_array, string_nofree_of("d"), init_rw_dict());
    rw_dict_add_dict(dicts_d2, string_nofree_of("array"), dicts_d2_array);
    rw_array_add_dict(dicts, dicts_d2);

    rw_dict_t *file_dict = init_rw_dict();
    rw_dict_add_array(file_dict, string_nofree_of("strings"), strings);
    rw_dict_add_array(file_dict, string_nofree_of("numbers"), numbers);
    rw_dict_add_array(file_dict, string_nofree_of("booleans"), booleans);
    rw_dict_add_array(file_dict, string_nofree_of("arrays"), arrays);
    rw_dict_add_array(file_dict, string_nofree_of("dicts"), dicts);

    rw_json_t *rw_json_manual = init_rw_json(false, 0, file_dict);

    cr_expect(rw_json_equal(rw_json, rw_json_manual),
              "Expected the 2 dicts to be equal, but they were not");

    destroy_rw_json(rw_json);
    destroy_rw_json(rw_json_manual);
}
