#include <iostream>
#include <boost/coroutine/all.hpp>

typedef boost::coroutines::asymmetric_coroutine< void >::pull_type pull_coro_t;
typedef boost::coroutines::asymmetric_coroutine< void >::push_type push_coro_t;


const int N = 1e10;

void foo(push_coro_t & sink)
{
    for (int i = 0; i < N; i++)
    {
        // printf("%d\n", i);
        sink();
    }
}

int main(int argc, char * argv[])
{
    {
        pull_coro_t source(foo);
        while (source)
        {
            source();
        }
    }

    std::cout << "Done" << std::endl;

    return 0;
}