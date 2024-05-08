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
    add_str(jd, "te", "a");
    add_str(jd, "tes", "a");
    add_str(jd, "test", "a");
    add_str(jd, "testi", "a");
    add_str(jd, "testin", "a");
    add_str(jd, "testing", "a");

    for (long i = 0; i < 10; ++i)
    {
        add_num(jd, "a", i);
    }
    print_pairs(jd->pairs);
    destroy_dict(jd);
    return 0;
}
