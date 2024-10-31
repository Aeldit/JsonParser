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
        arr_print(j->array);
    }
    else if (IS_DICT(j))
    {
        dict_print(j->dict);
    }
    destroy_json(j);
    return 0;
}
