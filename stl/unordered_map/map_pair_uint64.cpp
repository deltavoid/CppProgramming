#include <cstdio>
#include <cstdlib>
#include <cstdint>


#include <memory>
#include <utility>
#include <unordered_map>
#include <functional>



typedef std::pair<uint64_t, uint64_t> key_type;

struct key_hash {


};





int main()
{
    std::unordered_map<std::pair<uint64_t, uint64_t>, uint64_t, 
            std::function<size_t(const std::pair<uint64_t, uint64_t>)>>

    map(


}