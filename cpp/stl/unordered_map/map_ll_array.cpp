#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_map>

typedef long long ll;

struct Value
{
    ll array[32];
    // Value() { memset(array, 0, sizeof(array));}
};

int main()
{
    typedef std::unordered_map<ll, Value> map_type;
    map_type map;

    Value val;
    val.array[0] = 1;

    std::pair<map_type::iterator, bool> result1 =  map.emplace(1, val);
    printf("result1: %d\n", (int)(result1.second));

    map[2].array[0] = 2;
    
    // std::pair<map_type::iterator, bool> result =  map.insert(std::make_pair<ll, ll>(1, 2));
    // printf("result: %d\n", (int)(result.second));


    for (map_type::iterator it = map.begin(); it != map.end(); it++)
        printf("%lld %lld\n", it->first, it->second.array[0]);
    printf("hello world\n");
    return 0;
}