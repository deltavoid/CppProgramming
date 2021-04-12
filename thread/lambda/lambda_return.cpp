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



    return 0;
}