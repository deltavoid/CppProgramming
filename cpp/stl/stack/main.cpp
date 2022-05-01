#include <cstdio>
#include <cstdlib>

#include <utility>
#include <stack>



typedef std::pair<int, int> Point;

int main(int argc, char** argv, char** env)
{
    // printf("env:\n");
    // for (int i = 0; env[i] != NULL; i++)
    //     printf("%s\n", env[i]);
    // printf("\n");

    // printf("arg:\n");
    // for (int i = 0; i < argc; i++)
    //     printf("%s\n", argv[i]);

    std::stack<Point> s;

    for (int i = 0; i < 3; i++)
        s.push(Point(i, i * 2));


    // compile error, looks stack has not iterator, suggest deque
    // std::stack<Point>::iterator it;
    // for (it = s.begin(); it != s.end(); it++)
    //     printf("%d %d\n", it->first, it->second);

    return 0;
}