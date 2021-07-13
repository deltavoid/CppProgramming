#include <cstdio>
#include <cstdlib>
#include <cstdint>

#include <memory>
#include <utility>
#include <tuple>
#include <string>



int main()
{
    printf("hello world\n");


    std::tuple<int, float, std::string> tuple1(1, 1.0, "hello");
    printf("tuple1.e0: %d, tuple1.e1: %f, tuple1.e2: %s\n",
            std::get<0>(tuple1), std::get<1>(tuple1), std::get<2>(tuple1).c_str());



    return 0;
}