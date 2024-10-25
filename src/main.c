#include "printing.h"
#include "storage.h"

int main(int argc, char *argv[])
{
    if (argc != 2 || !argv)
    {
        return 1;
    }

    Array a = init_array(12);

    String s1 = { .str = "Test", .length = 4 };
    String s2 = { .str = "Hello", .length = 5 };

    arr_add_int(&a, 1);
    arr_add_int(&a, 2);
    arr_add_str(&a, s1);
    arr_add_str(&a, s2);
    arr_add_int(&a, 5);
    arr_add_null(&a);
    arr_add_null(&a);
    arr_add_bool(&a, 1);
    arr_add_bool(&a, 0);
    arr_add_double(&a, 0.5);
    arr_add_double(&a, 55.789);

    Array b = init_array(2);
    arr_add_int(&b, 9);
    arr_add_int(&b, 8);

    arr_add_arr(&a, &b);

    arr_print(&a);

    destroy_array(&a);
    return 0;
}
