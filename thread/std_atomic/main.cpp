#include <cstdio>
#include <cstdlib>
#include <atomic>


class Container
{public:
    std::atomic_int a;
    int b;

    Container() : a(0), b(0) {}

    void inc()  {  a++;}
    int get()  {  return (int)a;}

    void inc_b()  {  __sync_fetch_and_add(&b, 1);}
    int get_b()  {  return b;}

};


int main()
{
    std::atomic_int a(0);
    a += 1;
    printf("%d\n", (int)a);

    Container b;
    b.inc();
    printf("b: %d\n", b.get());

    b.inc_b();
    printf("b.b: %d\n", b.get_b());

    return 0;
}