#include <stdio.h>

#include "linked_list.h"
#include "parser.h"
#include "values_storage.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    printf("%s\n", argv[0]);

    json_st j = parse(argv[1]);
    if (j.is_array)
    {
        // print_array(ja);
        destroy_json_array(&j.ja);
    }
    else
    {
        destroy_json_dict(&j.jd);
        // print_json(jd->items);

        /*typed_value_st tv = get_value(jd, "array", 5);
        if (tv.type == TYPE_ARR)
        {
            print_array((json_array_st *)tv.value);
        }
        destroy_dict(jd);*/
    }
    destroy_storage(&j.storage);
    return 0;
}
