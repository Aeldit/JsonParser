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
    add_str(jd, "a", "a");
    add_str(jd, "b", "a");
    add_str(jd, "c", "a");
    add_str(jd, "d", "a");
    add_str(jd, "e", "a");
    add_str(jd, "f", "a");
    add_str(jd, "g", "a");
    add_str(jd, "h", "a");
    add_str(jd, "i", "a");
    add_str(jd, "j", "a");
    add_str(jd, "k", "a");
    add_str(jd, "l", "a");
    add_str(jd, "m", "a");
    print_pairs(jd->pairs);
    destroy_dict(jd);
    return 0;
}
