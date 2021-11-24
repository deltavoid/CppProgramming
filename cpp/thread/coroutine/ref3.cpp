#include <iostream>
#include <boost/coroutine/all.hpp>

typedef boost::coroutines::asymmetric_coroutine< int >::pull_type pull_coro_t;
typedef boost::coroutines::asymmetric_coroutine< int >::push_type push_coro_t;


void runit(pull_coro_t & source)
{
    printf("foo 1\n");
    std::cout << source.get() << std::endl;
    source();

    printf("foo 2\n");
    std::cout << source.get() << std::endl;
    source();

    printf("foo 3\n");
    std::cout << source.get() << std::endl;
    source();

    printf("foo 4\n");
    std::cout << source.get() << std::endl;
}

int main(int argc, char * argv[])
{
    {
        printf("main 1\n");
        push_coro_t sink(runit);
        printf("main 2\n");

        int i = 0;
        while (sink)
        {
            printf("main 3\n");
            ++i;
            sink(i);
            printf("main 4\n");
            // std::cout << "-";
            // printf("main 5\n");
        }
    }

    std::cout << "Done" << std::endl;

    return 0;
}