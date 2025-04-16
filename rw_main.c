#include <stdio.h>

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

    rw_json_t j = rw_parse(argv[1]);
    if (j.is_array)
    {
        printf("\n");

        rw_array_t a = init_rw_array_with(
            5, RWVAL_LONG(1), RWVAL_LONG(2), RWVAL_LONG(3), RWVAL_LONG(4),
            RWVAL_LONG(5)
        );
        rw_array_add_array(&j.array, a);
        rw_array_remove(&a, 2);
        rw_value_t v = rw_array_get(j.array, 0);
        rw_item_t it = rw_dict_get(v.dictv, string_nofree_of("emojis"));
        if (it.type == T_STR)
        {
            printf("BBB %s\n", it.strv.str);
        }
        else
        {
            printf("AAA %d", it.type);
        }
        write_rw_json_to_file(j, "out.json");
    }
    rw_json_print(j);
    destroy_rw_json(j);
    return 0;
}
