#include <cstdio>
#include <cstdlib>

#include <thread>
#include <memory>
#include <vector>

#include <unistd.h>


typedef long long ll;

class PostUpdatingQueue
{public:
    ll id;
    bool running = true;
    std::unique_ptr<std::thread> thread;

    PostUpdatingQueue(ll id);
    ~PostUpdatingQueue();

    void run();
};

PostUpdatingQueue::PostUpdatingQueue(ll id)
    : id(id)
{
    printf("PostUpdatingQueue: %lld\n", id);
    running = true;
    thread = std::make_unique<std::thread>(  [this]() { this->run();}  );
}

PostUpdatingQueue::~PostUpdatingQueue()
{
    running = false;
    thread->join();
    printf("~PostUpdatingQueue: %lld\n", id);
}

void PostUpdatingQueue::run()
{
    // for (ll i = 0; i < 10; i++)
    // {
    //     printf("run: %lld\n", id);
    //     sleep(1);
    // }
    ll cnt = 0;
    while (running)
    {
        printf("%ld run %lld\n", id, cnt++);
        sleep(1);
    }
        
}


const ll threads = 10;

int main()
{
    // std::vector<PostUpdatingQueue> puqs(10);
    // sleep(10);
    std::vector<std::unique_ptr<PostUpdatingQueue>> puqs(threads);
    for (ll i = 0; i < threads; i++)
        puqs[i] = std::make_unique<PostUpdatingQueue>(i);

    for (ll i = 0; i < puqs.size(); i++)
        puqs[i]->running = false;

    return 0;
}