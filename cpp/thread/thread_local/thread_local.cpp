#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <thread>
#include <memory>


#include <unistd.h>


// int count = 0;

struct context {

    int cnt;

    static int count;

    context()
    {
        printf("context::constructor, count: %d\n", ++count);
    }

    ~context()
    {
        printf("context::destructor, count: %d\n", --count);
    }
};

int context::count = 0;




thread_local struct context local_context;


struct context& get_context()
{
    // struct context* ctx = local_context;

    // if  (ctx == NULL)
    // {
    //     local_context = new context();

    //     ctx = local_context;
    // }

    // return ctx;
    return local_context;
}



void worker()
{
    struct context& ctx = get_context();

    printf("ctx is %lx, count is %d\n", &ctx, context::count);

    sleep(1);
}



int main()
{

    for (int i = 0; i < 5; i++)
    {
        std::thread thread(worker);

        thread.join();

        printf("\n");
    }

    return 0;
}