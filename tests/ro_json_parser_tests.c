#include <criterion/criterion.h>
#include <stdio.h>
#include <sys/stat.h>

#include "../src/base_json_parser.h"
#include "../src/base_json_storage.h"
#include "../src/ro_json_equality.h"
#include "../src/ro_json_parser.h"

#define JSON_TESTS_FILE ("tests/test.json")

/*******************************************************************************
**                              RO_PARSE_ARRAY_BUFF                           **
*******************************************************************************/
ro_array_t *get_array_from_file(unsigned long *idx, char is_buff)
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

    if (is_buff)
    {
        if (fseek(f, 0, SEEK_SET) != 0)
        {
            fclose(f);
            return 0;
        }

        struct stat st;
        stat(JSON_TESTS_FILE, &st);
        unsigned long nb_chars = st.st_size;

        buff = calloc(nb_chars + 1, sizeof(char));
        if (!buff)
        {
            fclose(f);
            return 0;
        }
        fread(buff, sizeof(char), nb_chars, f);

        a = ro_parse_array_buff(buff, idx);
    }
    else
    {
        if (fseek(f, *idx, SEEK_SET) != 0)
        {
            fclose(f);
            return 0;
        }
        a = ro_parse_array(f, idx);
    }

    if (buff)
    {
        free(buff);
    }
    fclose(f);
    return a;
}

Test(ro_json_parser, ro_parse_array_buff_empty)
{
    unsigned long idx = 518;
    ro_array_t *a = get_array_from_file(&idx, 1);

    ro_array_t *b = init_ro_array(0);
    char is_equal = ro_arrays_equal(a, b);

    cr_expect(is_equal, "Expected the 2 arrays to be equal, but they were not");
    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_json_parser, ro_parse_array_buff_long_numbers)
{
    unsigned long idx = 85;
    ro_array_t *a = get_array_from_file(&idx, 1);

    ro_array_t *b = init_ro_array(20);

    ro_array_add_long(b, 64220);
    ro_array_add_long(b, -512);

    ro_array_add_double(b, 642.25);
    ro_array_add_double(b, -642.25);

    ro_array_add_exp_long(b, EXP_LONG_OF(2, 8));
    ro_array_add_exp_long(b, EXP_LONG_OF(-53, 4));
    ro_array_add_exp_long(b, EXP_LONG_OF(200, -10));
    ro_array_add_exp_long(b, EXP_LONG_OF(-251, -10));

    ro_array_add_exp_double(b, EXP_DOUBLE_OF(200.5, 10));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(-200.5, 10));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(200.5, -10));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(-200.5, -10));

    ro_array_add_exp_long(b, EXP_LONG_OF(2, 8));
    ro_array_add_exp_long(b, EXP_LONG_OF(-53, 4));
    ro_array_add_exp_long(b, EXP_LONG_OF(200, -10));
    ro_array_add_exp_long(b, EXP_LONG_OF(-251, -10));

    ro_array_add_exp_double(b, EXP_DOUBLE_OF(200.5, 10));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(-200.5, 10));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(200.5, -10));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(-200.5, -10));

    char is_equal = ro_arrays_equal(a, b);

    cr_expect(is_equal, "Expected the 2 arrays to be equal, but they were not");
    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_json_parser, ro_parse_array_buff_nested_multi_type)
{
    unsigned long idx = 508;
    ro_array_t *a = get_array_from_file(&idx, 1);

    ro_array_t *b = init_ro_array(8);

    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_long(b, 1);
    ro_array_add_long(b, 2);
    ro_array_add_long(b, 3);

    ro_array_t *a2 = init_ro_array(4);
    ro_array_add_str(a2, string_nofree_of(""));
    ro_array_add_long(a2, 5);

    ro_array_t *a3 = init_ro_array(3);
    ro_array_add_str(a3, string_nofree_of("dd"));
    ro_array_add_bool(a3, 1);
    ro_array_add_long(a3, 5);

    ro_array_add_array(a2, a3);
    ro_array_add_dict(a2, init_ro_dict(0));

    ro_array_add_array(b, a2);
    ro_array_add_dict(b, init_ro_dict(0));

    ro_dict_t *d = init_ro_dict(1);
    ro_dict_add_str(d, string_nofree_of("first"), string_nofree_of("w"));

    ro_array_add_dict(b, d);
    ro_array_add_dict(b, init_ro_dict(0));

    char is_equal = ro_arrays_equal(a, b);

    cr_expect(is_equal, "Expected the 2 arrays to be equal, but they were not");
    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_json_parser, ro_parse_array_empty)
{
    unsigned long idx = 519;
    ro_array_t *a = get_array_from_file(&idx, 0);

    ro_array_t *b = init_ro_array(0);
    char is_equal = ro_arrays_equal(a, b);

    cr_expect(is_equal, "Expected the 2 arrays to be equal, but they were not");
    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_json_parser, ro_parse_array_long_numbers)
{
    unsigned long idx = 86;
    ro_array_t *a = get_array_from_file(&idx, 0);

    ro_array_t *b = init_ro_array(20);

    ro_array_add_long(b, 64220);
    ro_array_add_long(b, -512);

    ro_array_add_double(b, 642.25);
    ro_array_add_double(b, -642.25);

    ro_array_add_exp_long(b, EXP_LONG_OF(2, 8));
    ro_array_add_exp_long(b, EXP_LONG_OF(-53, 4));
    ro_array_add_exp_long(b, EXP_LONG_OF(200, -10));
    ro_array_add_exp_long(b, EXP_LONG_OF(-251, -10));

    ro_array_add_exp_double(b, EXP_DOUBLE_OF(200.5, 10));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(-200.5, 10));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(200.5, -10));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(-200.5, -10));

    ro_array_add_exp_long(b, EXP_LONG_OF(2, 8));
    ro_array_add_exp_long(b, EXP_LONG_OF(-53, 4));
    ro_array_add_exp_long(b, EXP_LONG_OF(200, -10));
    ro_array_add_exp_long(b, EXP_LONG_OF(-251, -10));

    ro_array_add_exp_double(b, EXP_DOUBLE_OF(200.5, 10));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(-200.5, 10));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(200.5, -10));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(-200.5, -10));

    char is_equal = ro_arrays_equal(a, b);

    cr_expect(is_equal, "Expected the 2 arrays to be equal, but they were not");
    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_json_parser, ro_parse_array_nested_multi_type)
{
    unsigned long idx = 509;
    ro_array_t *a = get_array_from_file(&idx, 0);

    ro_array_t *b = init_ro_array(8);

    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_long(b, 1);
    ro_array_add_long(b, 2);
    ro_array_add_long(b, 3);

    ro_array_t *a2 = init_ro_array(4);
    ro_array_add_str(a2, string_nofree_of(""));
    ro_array_add_long(a2, 5);

    ro_array_t *a3 = init_ro_array(3);
    ro_array_add_str(a3, string_nofree_of("dd"));
    ro_array_add_bool(a3, 1);
    ro_array_add_long(a3, 5);

    ro_array_add_array(a2, a3);
    ro_array_add_dict(a2, init_ro_dict(0));

    ro_array_add_array(b, a2);
    ro_array_add_dict(b, init_ro_dict(0));

    ro_dict_t *d = init_ro_dict(1);
    ro_dict_add_str(d, string_nofree_of("first"), string_nofree_of("w"));

    ro_array_add_dict(b, d);
    ro_array_add_dict(b, init_ro_dict(0));

    char is_equal = ro_arrays_equal(a, b);

    cr_expect(is_equal, "Expected the 2 arrays to be equal, but they were not");
    destroy_ro_array(a);
    destroy_ro_array(b);
}

/*******************************************************************************
**                               RO_PARSE_DICT_BUFF                           **
*******************************************************************************/
ro_dict_t *get_dict_from_file(unsigned long *idx, char is_buff)
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

    if (is_buff)
    {
        if (fseek(f, 0, SEEK_SET) != 0)
        {
            fclose(f);
            return 0;
        }

        struct stat st;
        stat(JSON_TESTS_FILE, &st);
        unsigned long nb_chars = st.st_size;

        buff = calloc(nb_chars + 1, sizeof(char));
        if (!buff)
        {
            fclose(f);
            return 0;
        }
        fread(buff, sizeof(char), nb_chars, f);

        a = ro_parse_dict_buff(buff, idx);
    }
    else
    {
        if (fseek(f, *idx, SEEK_SET) != 0)
        {
            fclose(f);
            return 0;
        }
        a = ro_parse_dict(f, idx);
    }

    if (buff)
    {
        free(buff);
    }
    fclose(f);
    return a;
}

Test(ro_json_parser, ro_parse_dict_buff_empty)
{
    unsigned long idx = 812;
    ro_dict_t *a = get_dict_from_file(&idx, 1);

    ro_dict_t *b = init_ro_dict(0);
    char is_equal = ro_dicts_equal(a, b);

    cr_expect(is_equal, "Expected the 2 dicts to be equal, but they were not");
    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_json_parser, ro_parse_dict_buff_numbers)
{
    unsigned long idx = 824;
    ro_dict_t *a = get_dict_from_file(&idx, 1);

    ro_dict_t *b = init_ro_dict(3);

    ro_dict_add_long(b, string_nofree_of("1"), 1);
    ro_dict_add_long(b, string_nofree_of("2"), 2);
    ro_dict_add_long(b, string_nofree_of("3"), 3);

    char is_equal = ro_dicts_equal(a, b);

    cr_expect(is_equal, "Expected the 2 dicts to be equal, but they were not");
    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_json_parser, ro_parse_dict_buff_nested_multi_type)
{
    unsigned long idx = 905;
    ro_dict_t *a = get_dict_from_file(&idx, 1);

    ro_array_t *a3 = init_ro_array(3);
    ro_array_add_str(a3, string_nofree_of("dd"));
    ro_array_add_bool(a3, 1);
    ro_array_add_long(a3, 5);

    ro_dict_t *a2 = init_ro_dict(3);
    ro_dict_add_long(a2, string_nofree_of(""), 1);
    ro_dict_add_array(a2, string_nofree_of("arr"), a3);
    ro_dict_add_dict(a2, string_nofree_of("d"), init_ro_dict(0));

    ro_dict_t *b = init_ro_dict(1);
    ro_dict_add_dict(b, string_nofree_of("array"), a2);

    char is_equal = ro_dicts_equal(a, b);

    cr_expect(is_equal, "Expected the 2 dicts to be equal, but they were not");
    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_json_parser, ro_parse_dict_empty)
{
    unsigned long idx = 813;
    ro_dict_t *a = get_dict_from_file(&idx, 0);

    ro_dict_t *b = init_ro_dict(0);
    char is_equal = ro_dicts_equal(a, b);

    cr_expect(is_equal, "Expected the 2 dicts to be equal, but they were not");
    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_json_parser, ro_parse_dict_numbers)
{
    unsigned long idx = 825;
    ro_dict_t *a = get_dict_from_file(&idx, 0);

    ro_dict_t *b = init_ro_dict(3);

    ro_dict_add_long(b, string_nofree_of("1"), 1);
    ro_dict_add_long(b, string_nofree_of("2"), 2);
    ro_dict_add_long(b, string_nofree_of("3"), 3);

    char is_equal = ro_dicts_equal(a, b);

    cr_expect(is_equal, "Expected the 2 dicts to be equal, but they were not");
    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_json_parser, ro_parse_dict_nested_multi_type)
{
    unsigned long idx = 906;
    ro_dict_t *a = get_dict_from_file(&idx, 0);

    ro_array_t *a3 = init_ro_array(3);
    ro_array_add_str(a3, string_nofree_of("dd"));
    ro_array_add_bool(a3, 1);
    ro_array_add_long(a3, 5);

    ro_dict_t *a2 = init_ro_dict(3);
    ro_dict_add_long(a2, string_nofree_of(""), 1);
    ro_dict_add_array(a2, string_nofree_of("arr"), a3);
    ro_dict_add_dict(a2, string_nofree_of("d"), init_ro_dict(0));

    ro_dict_t *b = init_ro_dict(1);
    ro_dict_add_dict(b, string_nofree_of("array"), a2);

    char is_equal = ro_dicts_equal(a, b);

    cr_expect(is_equal, "Expected the 2 dicts to be equal, but they were not");
    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

/*******************************************************************************
**                                   RO_PARSE                                 **
*******************************************************************************/
Test(ro_json_parser, ro_parse)
{
    ro_json_t *ro_json = ro_parse(JSON_TESTS_FILE);
    if (!ro_json)
    {
        return;
    }

    ro_array_t *strings = init_ro_array(2);
    ro_array_add_str(strings, string_nofree_of("testing normal string"));
    ro_array_add_str(strings, string_nofree_of(""));

    ro_array_t *numbers = init_ro_array(20);
    ro_array_add_long(numbers, 64220);
    ro_array_add_long(numbers, -512);
    ro_array_add_double(numbers, 642.25);
    ro_array_add_double(numbers, -642.25);
    ro_array_add_exp_long(numbers, EXP_LONG_OF(2, 8));
    ro_array_add_exp_long(numbers, EXP_LONG_OF(-53, 4));
    ro_array_add_exp_long(numbers, EXP_LONG_OF(200, -10));
    ro_array_add_exp_long(numbers, EXP_LONG_OF(-251, -10));
    ro_array_add_exp_double(numbers, EXP_DOUBLE_OF(200.5, 10));
    ro_array_add_exp_double(numbers, EXP_DOUBLE_OF(-200.5, 10));
    ro_array_add_exp_double(numbers, EXP_DOUBLE_OF(200.5, -10));
    ro_array_add_exp_double(numbers, EXP_DOUBLE_OF(-200.5, -10));
    ro_array_add_exp_long(numbers, EXP_LONG_OF(2, 8));
    ro_array_add_exp_long(numbers, EXP_LONG_OF(-53, 4));
    ro_array_add_exp_long(numbers, EXP_LONG_OF(200, -10));
    ro_array_add_exp_long(numbers, EXP_LONG_OF(-251, -10));
    ro_array_add_exp_double(numbers, EXP_DOUBLE_OF(200.5, 10));
    ro_array_add_exp_double(numbers, EXP_DOUBLE_OF(-200.5, 10));
    ro_array_add_exp_double(numbers, EXP_DOUBLE_OF(200.5, -10));
    ro_array_add_exp_double(numbers, EXP_DOUBLE_OF(-200.5, -10));

    ro_array_t *booleans = init_ro_array(2);
    ro_array_add_bool(booleans, 1);
    ro_array_add_bool(booleans, 0);

    ro_array_t *arrays = init_ro_array(8);
    ro_array_add_array(arrays, init_ro_array(0));
    ro_array_add_long(arrays, 1);
    ro_array_add_long(arrays, 2);
    ro_array_add_long(arrays, 3);
    ro_array_t *arrays_a2 = init_ro_array(4);
    ro_array_add_str(arrays_a2, string_nofree_of(""));
    ro_array_add_long(arrays_a2, 5);
    ro_array_t *arrays_a3 = init_ro_array(3);
    ro_array_add_str(arrays_a3, string_nofree_of("dd"));
    ro_array_add_bool(arrays_a3, 1);
    ro_array_add_long(arrays_a3, 5);
    ro_array_add_array(arrays_a2, arrays_a3);
    ro_array_add_dict(arrays_a2, init_ro_dict(0));
    ro_array_add_array(arrays, arrays_a2);
    ro_array_add_dict(arrays, init_ro_dict(0));
    ro_dict_t *arrays_d = init_ro_dict(1);
    ro_dict_add_str(arrays_d, string_nofree_of("first"), string_nofree_of("w"));
    ro_array_add_dict(arrays, arrays_d);
    ro_array_add_dict(arrays, init_ro_dict(0));

    ro_array_t *dicts = init_ro_array(3);
    ro_array_add_dict(dicts, init_ro_dict(0));
    ro_dict_t *dicts_d1 = init_ro_dict(3);
    ro_dict_add_long(dicts_d1, string_nofree_of("1"), 1);
    ro_dict_add_long(dicts_d1, string_nofree_of("2"), 2);
    ro_dict_add_long(dicts_d1, string_nofree_of("3"), 3);
    ro_array_add_dict(dicts, dicts_d1);
    ro_dict_t *dicts_d2 = init_ro_dict(1);
    ro_dict_t *dicts_d2_array = init_ro_dict(3);
    ro_dict_add_long(dicts_d2_array, string_nofree_of(""), 1);
    ro_array_t *dicts_d2_array_arr = init_ro_array(3);
    ro_array_add_str(dicts_d2_array_arr, string_nofree_of("dd"));
    ro_array_add_bool(dicts_d2_array_arr, 1);
    ro_array_add_long(dicts_d2_array_arr, 5);
    ro_dict_add_array(dicts_d2_array, string_nofree_of("arr"),
                      dicts_d2_array_arr);
    ro_dict_add_dict(dicts_d2_array, string_nofree_of("d"), init_ro_dict(0));
    ro_dict_add_dict(dicts_d2, string_nofree_of("array"), dicts_d2_array);
    ro_array_add_dict(dicts, dicts_d2);

    ro_dict_t *file_dict = init_ro_dict(5);
    ro_dict_add_array(file_dict, string_nofree_of("strings"), strings);
    ro_dict_add_array(file_dict, string_nofree_of("numbers"), numbers);
    ro_dict_add_array(file_dict, string_nofree_of("booleans"), booleans);
    ro_dict_add_array(file_dict, string_nofree_of("arrays"), arrays);
    ro_dict_add_array(file_dict, string_nofree_of("dicts"), dicts);

    ro_json_t *ro_json_manual = init_ro_json(0, 0, file_dict);

    cr_expect(ro_json_equal(ro_json, ro_json_manual),
              "Expected the 2 dicts to be equal, but they were not");

    destroy_ro_json(ro_json);
    destroy_ro_json(ro_json_manual);
}
