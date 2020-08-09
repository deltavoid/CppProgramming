#include <cstdio>
#include <cstdlib>
#include <vector>


typedef long long ll;

int main()
{
    std::vector<ll> vec;

    vec.reserve(1000);
    printf("size: %lld capicity: %lld\n", vec.size(), vec.capacity());

    vec.resize(100);
    printf("size: %lld capicity: %lld\n", vec.size(), vec.capacity());

    vec.shrink_to_fit();
    printf("size: %lld capicity: %lld\n", vec.size(), vec.capacity());

    vec.clear();
    vec.shrink_to_fit();
    printf("size: %lld capicity: %lld\n", vec.size(), vec.capacity());


    return 0;
}