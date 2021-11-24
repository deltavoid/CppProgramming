#include <cstdio>
#include <cstdlib>

#include <thread>
#include <memory>

// #include <unistd.h>


class EventLoop
{public:
    std::unique_ptr<std::thread> thread;

    EventLoop()
    {
        printf("enter ctor\n");
        
        // 停止由线程自我停止，线程可放入对象内部
        thread = std::make_unique<std::thread>( [this] () {  this->run();} );
        printf("leave ctor\n");
    }

    ~EventLoop()
    {
        printf("enter dtor\n");
        
        thread->join();
        printf("leave dtor\n");
    }

    void run()
    {
        for (int i = 0; i < 5; i++)
        {
            printf("run %d\n", i);
            // sleep(1);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }    
    }
};


int main()
{
    EventLoop loop;

    return 0;
}