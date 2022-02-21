
#include <coroutine>

#include <cstdio>
#include <iostream>


struct foo_return
{
    using T = foo_return;
    struct promise_type
    {
        T get_return_object()
        {
            printf("get_return_object\n");
            return {};
        }
        
        std::suspend_never initial_suspend()
        {
            printf("initial_suspend\n");
            return {};
        }
        
        std::suspend_never final_suspend() noexcept
        {
            printf("final_suspend\n");
            return {};
        }
        
        void unhandled_exception()
        {
            printf("unhandled_exception\n");
        }
        
        void return_void()
        {
            printf("return_void\n");
        }
    };
};

struct suspend_custom
{
    std::coroutine_handle<> handle;

    bool await_ready() const noexcept
    {
        // std::cout << __FUNCTION__ << ": " << __LINE__ << std::endl;
        printf("await_ready\n");
        return false;
    }
    
    void await_suspend(std::coroutine_handle<> handle)
    {
        printf("await_suspend\n");
        // std::cout << __FUNCTION__ << ": " << std::endl;
        this->handle = std::coroutine_handle<>::from_address(handle.address());
    }
    
    void await_resume() const noexcept
    {
        printf("await_resume\n");
        // std::cout << __FUNCTION__ << ": " << std::endl;
    }
};

foo_return foo(suspend_custom& a)
{
    printf("foo: 1\n");
    for (int i = 0; i < 100; ++i)
    {
        // std::cout << i * 10000 + __LINE__ << std::endl;
        // co_await a;
        // std::cout << i * 10000 + __LINE__ << std::endl;
        // co_await a;
        // std::cout << i * 10000 + __LINE__ << std::endl;
        // co_await a;

        printf("foo: 2, i: %d\n", i);

        co_await a;

        printf("foo: 3, i: %d\n", i);

    }

    printf("foo: 4, end\n");    
    co_return;
}

int main()
{
    printf("main: 1\n");
    suspend_custom a;


    // initial coroutine
    printf("main: 2\n");
    foo(a);

    printf("main: 3\n");
    for(int i = 0; i < 40; ++i)
    {
        // resume coroutine
        printf("main: 4, i: %d\n", i);
        a.handle();
        printf("main: 5, i: %d\n", i);
        printf("\n");
    }

    printf("main: 6\n");
    a.handle.destroy();

    printf("main: 7, end\n");
    return 0;
}
