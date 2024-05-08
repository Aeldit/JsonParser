#include <stdio.h>
#include <stdlib.h>

#include "../src/linked_lists.h"

int main(void)
{
    key_control_t *kc = calloc(1, sizeof(key_control_t *));
    if (kc == NULL)
    {
        return 1;
    }
    append_key(kc, "t");
    append_key(kc, "te");
    append_key(kc, "tes");
    append_key(kc, "test");
    append_key(kc, "testi");
    append_key(kc, "testin");
    append_key(kc, "testing");
    append_key(kc, "a");
    append_key(kc, "b");
    append_key(kc, "c");
    append_key(kc, "d");
    append_key(kc, "e");
    append_key(kc, "f");
    append_key(kc, "g");
    append_key(kc, "h");
    append_key(kc, "i");
    append_key(kc, "j");
    append_key(kc, "k");
    append_key(kc, "l");
    append_key(kc, "m");
    print_keys(kc);
    destroy_key_control(kc);
    return 0;
}
