#include <iostream>
#include <boost/coroutine/all.hpp>

typedef boost::coroutines::asymmetric_coroutine< void >::pull_type pull_coro_t;
typedef boost::coroutines::asymmetric_coroutine< void >::push_type push_coro_t;


const int N = 100;

void foo(push_coro_t & sink)
{
    // std::cout << "1";
    // sink();
    // std::cout << "2";
    // sink();
    // std::cout << "3";
    // sink();
    // std::cout << "4";
    for (int i = 0; i < N; i++)
    {
        std::cout << i << " : ";
        for (int j = 0; j < 1024; j++)
            std::cout << (char)('a' + (i + j) % 26);
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
            std::cout << "\n";
        }
    }

    std::cout << "Done" << std::endl;

    return 0;
}