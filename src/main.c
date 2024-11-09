#include <stdio.h>
#include <stdlib.h>

#include "json_parser.h"
#include "json_printing.h"
#include "json_storage.h"
#include "json_write.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    JSON *j = parse(argv[1]);
    if (IS_ARRAY(j))
    {
        arr_print(j->array);
        printf("\n");

        Array *a = calloc(1, sizeof(Array));
        if (a)
        {
            arr_add_int(a, 1);
            arr_add_int(a, 2);
            arr_add_int(a, 3);
            arr_add_int(a, 4);
            arr_add_int(a, 5);
            arr_add_arr(j->array, a);
            write_json_to_file(j, "out.json");
        }
    }
    else if (IS_DICT(j))
    {
        dict_print(j->dict);
    }
    destroy_json(j);
    return 0;
}
