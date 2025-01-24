#include <stdio.h>
#include <stdlib.h>

#include "src/base_json_storage.h"
#include "src/rw_json_parser.h"
#include "src/rw_json_storage.h"
#include "src/rw_json_writer.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    rw_json_t *j = rw_parse(argv[1]);
    if (IS_ARRAY(j))
    {
        printf("\n");

        rw_array_t *a = calloc(1, sizeof(rw_array_t));
        if (a)
        {
            rw_array_add_long(a, 1);
            rw_array_add_long(a, 2);
            rw_array_add_long(a, 3);
            rw_array_add_long(a, 4);
            rw_array_add_long(a, 5);
            rw_array_add_array(j->array, a);
            rw_array_remove(a, 2);
            write_rw_json_to_file(j, "out.json");
        }
        rw_array_print(j->array);
    }
    else if (IS_DICT(j))
    {
        rw_dict_print(j->dict);
    }
    destroy_rw_json(j);
    return 0;
}
