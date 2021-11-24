#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"

#include "hello_trait.h"
#include "hello_impl.h"


int main(int argc, char** argv, char** env)
{
    // printf("env:\n");
    // for (int i = 0; env[i] != NULL; i++)
    //     printf("%s\n", env[i]);
    // printf("\n");

    // printf("arg:\n");
    // for (int i = 0; i < argc; i++)
    //     printf("%s\n", argv[i]);

    struct hello_impl hello_impl_instance = {
        .id = 0
    };

    struct hello_trait hello_trait_instance = {
        .ops = &hello_impl_ops,
        .data = &hello_impl_instance
    };

    hello_trait_set_id(&hello_trait_instance, 1);
    
    hello_trait_print_id(&hello_trait_instance);




    return 0;
}