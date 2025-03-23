#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <threads.h>

#include "src/ro_json_parser.h"
#include "src/ro_json_storage.h"

int main(int argc, char *argv[])
{
    if (argc < 2 || !argv)
    {
        return 1;
    }

    ro_json_t *j = ro_parse(argv[1]);
    if (IS_ARRAY(j))
    {
        printf("\n");
        // write_ro_json_to_file(j, "out.json");
        ro_array_print(j->array);
    }
    else if (IS_DICT(j))
    {
        ro_dict_print(j->dict);
    }

    // thrd_sleep(&(struct timespec){ .tv_sec = 10 }, NULL);

    destroy_ro_json(j);
    return 0;
}
