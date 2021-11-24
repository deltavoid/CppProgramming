#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"

#include "hello.h"


int main(int argc, char** argv, char** env)
{
    // printf("env:\n");
    // for (int i = 0; env[i] != NULL; i++)
    //     printf("%s\n", env[i]);
    // printf("\n");

    // printf("arg:\n");
    // for (int i = 0; i < argc; i++)
    //     printf("%s\n", argv[i]);

    struct hello* obj = hello_new();
    hello_set_id(obj, 1);
    hello_display_id(obj);
    hello_delete(obj);


    return 0;
}