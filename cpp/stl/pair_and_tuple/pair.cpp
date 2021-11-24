#include <cstdio>
#include <cstdlib>
#include <cstdint>

#include <memory>
#include <utility>


int main(int argc, char** argv)
{
    std::pair<uint64_t, uint64_t> pair1(1, 2);
    printf("pair1.first: %llu, pair1.second: %llu\n", pair1.first, pair1.second);


    auto pair2 = std::make_pair(3llu, 4llu);
    printf("pair2.first: %llu, pair2.second: %llu\n", pair2.first, pair2.second);


    return 0;
}