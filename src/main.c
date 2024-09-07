#include "json_api.h"
#include "parser.h"
#include "printing.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    printf("%s\n", argv[0]);

    /*json_dict_st *jd = parse(argv[1]);
    if (jd == NULL)
    {
        return 1;
    }

    typed_value_st tv = get_value(jd, "array", 5);
    if (tv.type == TYPE_ARR)
    {
        print_array((json_array_st *)tv.value);
    }

    destroy_dict(jd);*/

    printf("%lu\n", sizeof(struct json_dict));
    return 0;
}
