
#include <coroutine>


#include <iostream>

struct foo_return
{
    using T = foo_return;
    struct promise_type
    {
        T get_return_object()
        {
            return {};
        }
        
        std::suspend_never initial_suspend()
        {
            return {};
        }
        
        std::suspend_never final_suspend() noexcept
        {
            return {};
        }
        
        void unhandled_exception()
        {
        }
        
        void return_void()
        {
        }
    };
};

struct suspend_custom
{
    std::coroutine_handle<> handle;

    bool await_ready() const noexcept
    {
        std::cout << __FUNCTION__ << ": " << __LINE__ << std::endl;
        return false;
    }
    
    void await_suspend(std::coroutine_handle<> handle)
    {
        std::cout << __FUNCTION__ << ": " << __LINE__ << std::endl;
        this->handle = std::coroutine_handle<>::from_address(handle.address());
    }
    
    void await_resume() const noexcept
    {
        std::cout << __FUNCTION__ << ": " << __LINE__ << std::endl;
    }
};

foo_return foo(suspend_custom& a)
{
    for (int i = 0; i < 100; ++i)
    {
        std::cout << i * 10000 + __LINE__ << std::endl;
        co_await a;
        std::cout << i * 10000 + __LINE__ << std::endl;
        co_await a;
        std::cout << i * 10000 + __LINE__ << std::endl;
        co_await a;
    }
    co_return;
}

int main()
{
    suspend_custom a;
    // initial coroutine
    foo(a);
    for(int i = 0; i < 40; ++i)
    {
    // resume coroutine
        a.handle();
    }
    a.handle.destroy();
    return 0;
}
