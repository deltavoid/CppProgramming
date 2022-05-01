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
    v.push_back(3);

    printf("v.size(): %d\n", v.size());

    printf("v.front(): %d\n", v.front());
    printf("v.back(): %d\n", v.back());


    std::vector<int>::iterator it;
    for (it = v.begin(); it != v.end(); it++)
        printf("%d ", *it);
    printf("\n");


    v.pop_back();

    for (int i = 0; i < v.size(); i++)
        printf("%d ", v[i]);
    printf("\n");


    std::vector<int>::reverse_iterator rit;
    for (rit = v.rbegin(); rit != v.rend(); rit++)
        printf("%d ", *rit);
    printf("\n");



    return 0;

}