#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>


#include <vector>



int main()
{
    std::vector<int> v;

    v.push_back(1);
    v.push_back(2);

    printf("v.size(): %d\n", v.size());

    printf("v.front(): %d\n", v.front());
    printf("v.back(): %d\n", v.back());


}