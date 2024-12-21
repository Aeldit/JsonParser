#include <stdio.h>

#include "src/ro_json_parser.h"
#include "src/ro_json_storage.h"
#include "src/ro_json_write.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    ro_json_t *j = ro_parse(argv[1]);
    if (IS_ARRAY(j))
    {
        // ro_array_print(j->array);
        printf("\n");
        write_ro_json_to_file(j, "out.json");
    }
    else if (IS_DICT(j))
    {
        ro_dict_print(j->dict);
    }
    destroy_ro_json(j);
    return 0;
}
