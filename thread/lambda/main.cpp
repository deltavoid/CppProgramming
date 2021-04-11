#include <cstdio>
#include <cstdlib>


int main(int argc, char** argv)
{
    printf("arg:\n");
    for (int i = 0; i < argc; i++)
        printf("%s\n", argv[i]);

    return 0;
}