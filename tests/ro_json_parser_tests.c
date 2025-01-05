#include <criterion/criterion.h>
#include <stdio.h>
#include <sys/stat.h>

#include "../src/base_json_parser.h"
#include "../src/ro_equality.h"
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

    cr_expect(is_equal, "Expected the 2 arrays to be equal, but they wasn't");
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

    cr_expect(is_equal, "Expected the 2 arrays to be equal, but they wasn't");
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
    ro_array_add_str(a2, STRING_NOFREE_OF("", 0));
    ro_array_add_long(a2, 5);

    ro_array_t *a3 = init_ro_array(3);
    ro_array_add_str(a3, STRING_NOFREE_OF("dd", 2));
    ro_array_add_bool(a3, 1);
    ro_array_add_long(a3, 5);

    ro_array_add_array(a2, a3);
    ro_array_add_dict(a2, init_ro_dict(0));

    ro_array_add_array(b, a2);
    ro_array_add_dict(b, init_ro_dict(0));

    ro_dict_t *d = init_ro_dict(1);
    ro_dict_add_str(d, STRING_NOFREE_OF("first", 5), STRING_NOFREE_OF("w", 1));

    ro_array_add_dict(b, d);
    ro_array_add_dict(b, init_ro_dict(0));

    char is_equal = ro_arrays_equal(a, b);

    cr_expect(is_equal, "Expected the 2 arrays to be equal, but they wasn't");
    destroy_ro_array(a);
    destroy_ro_array(b);
}
