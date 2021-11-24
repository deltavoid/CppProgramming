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

    typedef std::tuple<int, float, std::string> tuple_type;

    printf("tuple_size<tupletype>::value: %d\n", std::tuple_size<tuple_type>::value);

    // std::tupel_element<1, tuple_type>::type // float

    {
        int i;
        std::string s;
        std::tie(i, std::ignore, s) = tuple1;  //将t中的每一个元素和外部的一个变量进行绑定
        printf("i: %d, s: %s\n", i, s.c_str());
    }

    int n = 1;
    auto tuple3 = std::tuple_cat(std::make_tuple(4, 1.1, "hello"), std::tie(n));
    printf("tuple3.e0: %d, tuple3.e1: %f, tuple3.e2: %s, tuple3.e3: %d\n",
            std::get<0>(tuple3), std::get<1>(tuple3), std::get<2>(tuple3), std::get<3>(tuple3));


    return 0;
}