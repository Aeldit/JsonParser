#include "src/ro_json_parser.h"
#include "src/ro_json_storage.h"
#include "src/ro_json_writer.h"

int main(int argc, char *argv[])
{
    if (argc < 2 || !argv)
    {
        return 1;
    }

    ro_json_t j = ro_parse(argv[1]);
    if (j.is_array)
    {
        write_ro_json_to_file(j, "out.json");
        ro_array_print(j.array);
    }
    else
    {
        ro_dict_print(j.dict);
    }

    destroy_ro_json(&j);
    return 0;
}
