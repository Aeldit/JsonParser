#include "api.h"
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

    typed_value_st tv = get_value(jd, "array", 5);
    print_array((json_array_st *)tv.value);

    destroy_dict(jd);
    return 0;
}
