#include <cstdio>
#include <cstdlib>
#include <cstdint>

#include <utility>
#include <map>


typedef std::map<uint64_t, uint64_t> map_t;
typedef map_t::iterator map_iterator_t;


int main(int argc, char** argv, char** env)
{

    map_t m;


    std::pair<map_iterator_t, bool> insert_result = 
            m.insert(std::pair<uint64_t, uint64_t>(1, 2));
    printf("insert result: key: %llu, value: %llu, inserted: %d\n",
        insert_result.first->first, insert_result.first->second, insert_result.second);

    m[3] = 4;
    m[5] = 6;
    m[7] = 8;

    printf("m.size(): %d\n", m.size());


    map_iterator_t it;
    for (it = m.begin(); it != m.end(); it++)
        printf("key: %llu, value: %llu\n", it->first, it->second);

    
    map_iterator_t find_result = m.find(1);
    printf("find_result: key: %llu, value: %llu\n", 
            find_result->first, find_result->second);

    // judge exsistence usually use find instead of count
    int count_result = m.count(1);
    printf("count_result: %d\n", count_result);


    map_iterator_t lower_bound_result = m.lower_bound(3);
    printf("lower_bound 3 result: key: %llu, value: %llu\n", 
            lower_bound_result->first, lower_bound_result->second);

    map_iterator_t upper_bound_result = m.upper_bound(3);
    printf("upper_bound 3 result: key: %llu, value: %llu\n", 
            upper_bound_result->first, upper_bound_result->second);

    std::pair<map_iterator_t, map_iterator_t> equal_range_result = 
            m.equal_range(3);
    printf("equal_range lower bound result: key: %llu, value: %llu\n",
            equal_range_result.first->first, equal_range_result.first->second);
    printf("equal_range upper bound result: key: %llu, value: %llu\n",
            equal_range_result.second->first, equal_range_result.second->second);
    

    m.erase(insert_result.first);
    m.erase(3);

    printf("map size: %d\n", m.size());


    map_t::reverse_iterator rit;
    for (rit = m.rbegin(); rit != m.rend(); rit++);
        printf("key: %llu, value: %llu\n", rit->first, rit->second);

    // for (it = m.begin(); it != m.end(); it++)
    //     printf("key: %llu, value: %llu\n", it->first, it->second);


    return 0;
}