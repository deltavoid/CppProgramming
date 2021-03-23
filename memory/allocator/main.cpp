#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <thread>
#include <memory>
#include <string>
#include <iostream>



int main(int argc, char** argv)
{
    printf("arg:\n");
    for (int i = 0; i < argc; i++)
        printf("%s\n", argv[i]);



    std::allocator<std::string> alloc;

    auto p = alloc.allocate(10);

    auto q = p;

    alloc.construct(q++);
    alloc.construct(q++, 10, 'c');
    alloc.construct(q++, "hi");

    std::cout << p[0] << std::endl;
    std::cout << p[1] << std::endl;
    std::cout << p[2] << std::endl;

    while (q != p)
    {
        alloc.destroy(--q);
    }

    alloc.deallocate(p, 10);



    return 0;
}