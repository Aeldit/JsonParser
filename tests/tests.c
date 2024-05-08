#include <stdio.h>
#include <stdlib.h>

#include "../src/linked_lists.h"
#include "../src/parser.h"

int main(void)
{
    json_dict_t *jd = init_dict();
    if (jd == NULL)
    {
        return 1;
    }
    add_key(jd, "t");
    add_key(jd, "te");
    add_key(jd, "tes");
    add_key(jd, "test");
    add_key(jd, "testi");
    add_key(jd, "testin");
    add_key(jd, "testing");
    add_key(jd, "a");
    add_key(jd, "b");
    add_key(jd, "c");
    add_key(jd, "d");
    add_key(jd, "e");
    add_key(jd, "f");
    add_key(jd, "g");
    add_key(jd, "h");
    add_key(jd, "i");
    add_key(jd, "j");
    add_key(jd, "k");
    add_key(jd, "l");
    add_key(jd, "m");
    print_keys(jd->keys);
    destroy_dict(jd);
    return 0;
}
