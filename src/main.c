#include "parser.h"
#include "printing.h"
#include "storage.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    JSON *j = parse(argv[1]);
    if (IS_ARRAY(j))
    {
        Array *a = j->array;
        for (unsigned i = 0; i < 40; ++i)
        {
            arr_add_int(a, i);
        }
        arr_print_array(a);
        for (unsigned i = 0; i < 18; ++i)
        {
            arr_remove(a, i + 2);
        }
        arr_print_array(a);
        for (unsigned i = 40; i < 70; ++i)
        {
            arr_add_int(a, i);
        }
        arr_print_array(a);
        for (unsigned i = 0; i < 6; ++i)
        {
            arr_remove(a, i + 2);
        }
        arr_print_array(a);
    }
    else if (IS_DICT(j))
    {
        dict_print(j->dict);
    }
    destroy_json(j);
    return 0;
}
