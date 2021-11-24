#include <cstdio>
#include <cstdlib>

#include <thread>
#include <memory>

#include "hello.h"
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

    std::unique_ptr<Hello> hello = std::make_unique<HelloImpl>();

    hello->setId(1);
    hello->printId();

    return 0;
}