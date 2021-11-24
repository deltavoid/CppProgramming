#include <cstdio>
#include <cstdlib>




class Temp
{public:
    int id;
    static int cnt;

    Temp()
    {
        id = ++cnt;
        printf("Temp: %d\n", id);
    }
};

int Temp::cnt = 0;


void test()
{
    // static auto cache = new Temp();
    static Temp* cache = new Temp();

    // static Temp* cache;
    // cache = new Temp();
}

int main()
{
    for (int i = 0; i < 10; i++)
        test();
    
    return 0;
}