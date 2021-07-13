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


    auto tuple2 = std::make_tuple(2, 2.0f, std::string("world"));
    printf("tuple2.e0: %d, tuple2.e1: %f, tuple2.e2: %s\n",
            std::get<0>(tuple2), std::get<1>(tuple2), std::get<2>(tuple2).c_str());


    return 0;
}