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
    if (!j)
    {
        return 1;
    }

    if (j->is_array)
    {
        Array *a = j->array;
        arr_print(a);
    }
    else
    {
        Dict *d = j->dict;
        dict_print(d);
    }
    destroy_json(j);
    return 0;
}
