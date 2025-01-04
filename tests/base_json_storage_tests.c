#include <criterion/criterion.h>

#include "../src/base_json_storage.h"

/*******************************************************************************
**                                STRINGS_EQUAL                               **
*******************************************************************************/
Test(base_json_storage, strings_equals_returns_true)
{
    char is_equal =
        strings_equals(STRING_OF("testing", 7), STRING_OF("testing", 7));
    cr_expect(is_equal,
              "Expected 'strings_equals(STRING_OF(\"testing\", 7), "
              "STRING_OF(\"testing\", 7)' to be true, but it was %s",
              is_equal ? "true" : "false");
}

Test(base_json_storage, strings_equals_returns_false_with_diff_len)
{
    char is_equal =
        strings_equals(STRING_OF("testing", 7), STRING_OF("testin", 6));
    cr_expect(!is_equal,
              "Expected 'strings_equals(STRING_OF(\"testing\", 7), "
              "STRING_OF(\"testin\", 6)' to be false, but it was %s",
              is_equal ? "true" : "false");
}

Test(base_json_storage, strings_equals_returns_false_with_same_len_diff_str)
{
    char is_equal = strings_equals(STRING_OF("sos", 3), STRING_OF("bob", 3));
    cr_expect(!is_equal,
              "Expected 'strings_equals(STRING_OF(\"sos\", 3), "
              "STRING_OF(\"bob\", 3)' to be false, but it was %s",
              is_equal ? "true" : "false");
}

Test(base_json_storage, strings_equals_returns_false_with_str_except_last_char)
{
    char is_equal = strings_equals(STRING_OF("sos", 3), STRING_OF("sob", 3));
    cr_expect(!is_equal,
              "Expected 'strings_equals(STRING_OF(\"sos\", 3), "
              "STRING_OF(\"sob\", 3)' to be false, but it was %s",
              is_equal ? "true" : "false");
}
