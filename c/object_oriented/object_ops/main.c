#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "hello_impl.h"


int main(int argc, char** argv, char** env)
{
    printf("env:\n");
    for (int i = 0; env[i] != NULL; i++)
        printf("%s\n", env[i]);
    printf("\n");

    printf("arg:\n");
    for (int i = 0; i < argc; i++)
        printf("%s\n", argv[i]);

    
    struct hello_impl* hello = malloc(sizeof(struct hello_impl));
    hello_impl_init(hello, 1);

    struct hello_ops** hello_interface = &hello->ops;
    hello_abstract_set_id(hello_interface, 2);

    printf("hello id: %d\n", hello_abstract_get_id(hello_interface));


    return 0;
}