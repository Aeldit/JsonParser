#include "parser.h"
#include "printing.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    json_dict_st *jd = parse(argv[1]);
    if (jd == NULL)
    {
        return 1;
    }

    puts("");
    print_json(jd->items);
    destroy_dict(jd);
    return 0;
}
