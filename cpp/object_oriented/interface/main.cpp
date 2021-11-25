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


    Hello* hello1 = new HelloImpl();
    
    hello1->setId(1);
    hello1->printId();
    
    delete hello1;



    std::unique_ptr<Hello> hello2 = std::make_unique<HelloImpl>();

    hello2->setId(2);
    hello2->printId();


    return 0;
}