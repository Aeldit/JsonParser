#include <stddef.h>
#include <stdint.h>
#include <threads.h>

#include "src/ro_json_parser.h"
#include "src/ro_json_storage.h"
#include "src/ro_json_writer.h"

int main(int argc, char *argv[])
{
    if (argc < 2 || !argv)
    {
        return 1;
    }

    ro_json_t j = ro_parse(argv[1]);
    if (j.is_array)
    {
        // printf("\n");
        // write_ro_json_to_file(j, "out.json");
        ro_array_print(j.array);
    }
    else
    {
        ro_dict_print(j.dict);
    }

    // thrd_sleep(&(struct timespec){ .tv_sec = 10 }, NULL);

    destroy_ro_json(j);
    return 0;
}
