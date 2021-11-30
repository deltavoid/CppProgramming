#include <cstdio>
#include <cstdlib>


int main(int argc, char** argv, char** envv)
{
    printf("arg:\n");
    for (int i = 0; i < argc; i++)
        printf("%s\n", argv[i]);


    printf("\nenv:\n");
    char** p = envv;
    while (*p != NULL)
    {
        printf("%s\n", *p);
        p++;
    }

    return 0;
}