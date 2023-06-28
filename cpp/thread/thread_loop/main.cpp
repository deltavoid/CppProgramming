#include <cstdio>
#include <cstdlib>
#include <cstdint>


uint64_t a;


int main(int argc, char** argv, char** env)
{
    printf("env:\n");
    for (int i = 0; env[i] != NULL; i++)
        printf("%s\n", env[i]);
    printf("\n");

    printf("arg:\n");
    for (int i = 0; i < argc; i++)
        printf("%s\n", argv[i]);


    while (true)
    {
        a++;

        // if  (a % 1000000 == 0)
        //     printf("a: %llu\n", a);
    }

    return 0;
}