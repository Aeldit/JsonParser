#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("This program must take at least 1 argument (a path to a file)");
        return 1;
    }

    printf("%s\n", argv[1]);
    return 0;
}
