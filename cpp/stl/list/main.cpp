#include <cstdio>
#include <cstdlib>

#include <list>






int main(int argc, char** argv, char** env)
{

    std::list<int> values;

    values.push_back(1);
    values.push_back(2);
    values.push_back(3);


    std::list<int>::iterator it;
    for (it = values.begin(); it != values.end(); it++)
    {
        printf("%d ", *it);
    }
    printf("\n");


    return 0;
}