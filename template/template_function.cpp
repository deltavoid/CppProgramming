#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>

template<typename Elem>
std::ostream& display(std::ostream& out, const std::string& msg, const std::vector<Elem>& vec)
{
    out << msg;
    for (int i = 0; i < vec.size(); i++)
        out << vec[i];
    out << std::endl;
}

template<typename T>
void test(int x)
{
    T temp(x);
    std::cout << temp << std::endl;
}


int main()
{
    std::vector<int> vec = {0, 1, 2, 3, 4};
    std::string msg("hello ");

    display(std::cout, msg, vec);

    display<int>(std::cout, msg, vec);

    int x = 1;
    // test(x); // no matching function

    test<long long>(x);

    test<long long>(2);

    test<long long>((short)3);


    // printf("hello world\n");
    return 0;
}