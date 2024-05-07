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
    printf("%lu\n", append_key(kc, "t"));
    printf("%lu\n", append_key(kc, "te"));
    printf("%lu\n", append_key(kc, "tes"));
    printf("%lu\n", append_key(kc, "test"));
    printf("%lu\n", append_key(kc, "testi"));
    printf("%lu\n", append_key(kc, "testin"));
    printf("%lu\n", append_key(kc, "testing"));
    printf("%lu\n", append_key(kc, "a"));
    printf("%lu\n", append_key(kc, "b"));
    printf("%lu\n", append_key(kc, "c"));
    printf("%lu\n", append_key(kc, "d"));
    printf("%lu\n", append_key(kc, "e"));
    printf("%lu\n", append_key(kc, "f"));
    printf("%lu\n", append_key(kc, "g"));
    printf("%lu\n", append_key(kc, "h"));
    printf("%lu\n", append_key(kc, "i"));
    printf("%lu\n", append_key(kc, "j"));
    printf("%lu\n", append_key(kc, "k"));
    printf("%lu\n", append_key(kc, "l"));
    printf("%lu\n", append_key(kc, "m"));
    print_keys(kc);
    free(kc);
    return 0;
}
