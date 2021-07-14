#include <cstdio>
#include <cstdlib>
#include <cstdint>


#include <memory>
#include <unordered_map>








int main()
{
    std::unordered_map<uint64_t, std::shared_ptr<uint64_t>> map;



    std::shared_ptr<uint64_t> vp = nullptr;
    auto it = map.find(1);
    if  (it == map.end())
    {
        vp = std::make_shared<uint64_t>(2);
        map.insert({1, vp});
    }
    else
    {
        vp = it->second;
    }

    printf("vp: %llu\n", *vp);


}