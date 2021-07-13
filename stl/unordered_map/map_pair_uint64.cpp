#include <cstdio>
#include <cstdlib>
#include <cstdint>


#include <memory>
#include <utility>
#include <unordered_map>
#include <functional>



typedef std::pair<uint64_t, uint64_t> key_type;

struct key_hash {

    size_t operator() (const key_type& key) const 
    {
        return std::hash<uint64_t>()(key.first) 
                ^ std::hash<uint64_t>()(key.second);
    }
};





int main()
{
    std::unordered_map<key_type, uint64_t, key_hash> map;

    map.insert({{1, 2}, 3});

    auto it = map.begin();

    printf("key.first: %llu, key.second: %llu, value: %llu\n",
            it->first.first, it->first.second, it->second);


}