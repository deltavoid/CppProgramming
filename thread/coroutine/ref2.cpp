#include <iostream>
#include <boost/coroutine/all.hpp>

typedef boost::coroutines::asymmetric_coroutine< int >::pull_type pull_coro_t;
typedef boost::coroutines::asymmetric_coroutine< int >::push_type push_coro_t;


void runit(push_coro_t & sink1)
{
    std::cout << "foo 1" << std::endl;
    sink1(10);
    std::cout << "foo 2" << std::endl;
    sink1(20);
    std::cout << "foo 3" << std::endl;
    sink1(30);
    std::cout << "foo 4" << std::endl;
}

int main(int argc, char * argv[])
{
    {
        printf("main 1\n");
        pull_coro_t source(runit);
        printf("main 2\n");
        while (source)
        {
            printf("main 3\n");
            int ret = source.get();
            std::cout << "ret: " << ret << std::endl;
            printf("main 4\n");
            source();
            printf("main 5\n");
        }
        printf("main 6\n");
    }

    std::cout << "Done" << std::endl;

    return 0;
}