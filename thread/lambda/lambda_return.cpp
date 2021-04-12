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

        int a = []() {

            return 1;

        }();


        printf("a: %d\n", a);
    }
    printf("\n");

    return 0;
}