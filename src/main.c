#include <stdio.h>

#include "parser.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf(
            "%s: This program must take at least 1 argument (a path to a file)",
            argv[0]);
        return 1;
    }

    // json_dict jd = parse(argv[1]);
    printf("%lu %lu\n", sizeof(char), sizeof(unsigned short));
    return 0;
}
