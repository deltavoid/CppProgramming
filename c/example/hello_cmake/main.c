#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


int main(int argc, char** argv, char** env)
{
    printf("env:\n");
    for (int i = 0; env[i] != NULL; i++)
        printf("%s\n", env[i]);
    printf("\n");

    printf("arg:\n");
    for (int i = 0; i < argc; i++)
        printf("%s\n", argv[i]);

    return 0;
}