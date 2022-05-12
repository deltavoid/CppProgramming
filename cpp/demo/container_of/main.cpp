#include <cstdio>
#include <cstdlib>




#ifndef offsetof
    #define offsetof(type, member) \
        ((size_t)&reinterpret_cast<char const volatile&>((((type*)0)->member)))
#endif
 
#ifndef container_of
    #define container_of(ptr, type, member) \
        ((type*)((char*)static_cast<const decltype(((type*)0)->member)*>(ptr) - offsetof(type,member)))
#endif



struct abc
{
    int     a;
    short   b;
    int     c;
};
 
int main()
{
    abc* A = new abc();
    abc* B = container_of(&A->c, abc, c);
 
    return 0;
}

