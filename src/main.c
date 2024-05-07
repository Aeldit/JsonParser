#include <stdio.h>

#include "parser.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("This program must take at least 1 argument (a path to a file)");
        return 1;
    }

    struct fc_control *fcc = read_file(argv[1]);
    print_file_content(fcc);
    return 0;
}
