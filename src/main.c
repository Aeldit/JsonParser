#include "json_api.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    json_st j = parse(argv[1]);
    if (j.is_array)
    {
        // print_array(ja);
    }
    else
    {
        // print_json(jd->items);

        /*typed_value_st tv = get_value(jd, "array", 5);
        if (tv.type == TYPE_ARR)
        {
            print_array((json_array_st *)tv.value);
        }
        destroy_dict(jd);*/
    }
    destroy_json(&j);
    return 0;
}
