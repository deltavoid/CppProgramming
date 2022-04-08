#include <cstdio>
#include <cstdlib>

#include <fmt/format.h>


int main(int argc, char** argv, char** env)
{
    printf("env:\n");
    for (int i = 0; env[i] != NULL; i++)
        printf("%s\n", env[i]);
    printf("\n");

    printf("arg:\n");
    for (int i = 0; i < argc; i++)
        printf("%s\n", argv[i]);



    fmt::print("hello {}\n", "world");


    return 0;
}