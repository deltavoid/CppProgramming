#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>


#include <thread>
#include <memory>
#include <functional>



int main()
{
    printf("hello world\n");

    printf("1\n");
    {
        int a = 1, b = 2;

        int ret = [](int a, int b) {

            return a > b? a : b;

        }(a, b);


        printf("ret: %d\n", ret);
    }
    printf("\n");


    printf("2\n");
    {
        int a = 1, b = 2;

        int ret = [](int a, int b) {

            if  (a > b)
                return a;
            else
                return b;

        }(a, b);

        printf("ret: %d\n", ret);
    }
    printf("\n");


    printf("3\n");
    {
        int a = 1, b = 2;

        int ret = [](int a, int b) {

            if  (a > b)
                return (long long)a;
            else
                return (long long)b;

        }(a, b);

        printf("ret: %d\n", ret);
    }
    printf("\n");


    printf("4\n");
    {
        int a = 1, b = 2;

        long long ret = [](int a, int b) -> long long{

            if  (a > b)
                return a;
            else
                return b;

        }(a, b);

        printf("ret: %lld\n", ret);
    }
    printf("\n");

    return 0;
}