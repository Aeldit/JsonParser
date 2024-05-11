#include <stdio.h>
#include <stdlib.h>

#include "../src/parser.h"

int main(void)
{
    json_dict_st *jd = init_dict();
    if (jd == NULL)
    {
        return 1;
    }
    add_str(jd, "t", "a");
    add_str(jd, "te", "b");
    add_str(jd, "tes", "c");
    add_str(jd, "test", "d");
    add_str(jd, "testi", "e");
    add_str(jd, "testin", "f");
    add_str(jd, "testing", "g");

    for (long i = 0; i < 10; ++i)
    {
        add_num(jd, "a", i);
    }

    add_bool(jd, "bool1", 0);
    add_bool(jd, "bool2", 1);

    add_null(jd, "testing_nulls");

    print_json(jd->pairs);
    destroy_dict(jd);
    return 0;
}
