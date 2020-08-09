#include <cstdio>
#include <cstdlib>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <queue>

#include <unistd.h>

typedef long long ll;
typedef unsigned long long ull;


class BlockingQueue
{public:
    std::queue<ull> queue;
    std::mutex mutex;
    std::condition_variable not_empty;

    BlockingQueue()
    {
    }

    ~BlockingQueue()
    {
    }

    void put(ull x)
    {
        std::lock_guard<std::mutex> guard(mutex);
        queue.push(x);
        not_empty.notify_one();
    }

    ull take()
    {
        std::unique_lock<std::mutex> lock(mutex);
        not_empty.wait(lock, [this] { return !queue.empty();});
        
        ull x = queue.front();
        queue.pop();

        return x;
    }

    bool empty()
    {
        std::lock_guard<std::mutex> guard(mutex);
        return queue.empty();
    }

};

class Producer
{public:
    std::unique_ptr<std::thread> thread;
    BlockingQueue* queue;
    int num;

    Producer(BlockingQueue* q, int num)
        : queue(q), num(num) 
    {
        thread = std::make_unique<std::thread>( [this] { this->run();} );
    }

    ~Producer()
    {
        thread->join();
    }

    void run()
    {
        for (int i = 1; i <= num; i++)
        {
            queue->put(i);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        queue->put(0);
    }
};

class Consumer
{public:
    std::unique_ptr<std::thread> thread;
    BlockingQueue* queue;

    Consumer(BlockingQueue* q)
        : queue(q)
    {
        thread = std::make_unique<std::thread>( [this] { this->run();} );
    }

    ~Consumer() 
    {
        thread->join();
    }

    void run()
    {
        ull x = 0;
        while ((x = queue->take()) != 0)
        {
            printf("take: %llu\n", x);
        }
    }
};


int main()
{
    std::vector<std::unique_ptr<Producer>> producers;
    std::vector<std::unique_ptr<Consumer>> consumers;

    int producer_num = 4;
    int consumer_num = 4;
    int production_num = 10;
    BlockingQueue queue;
    for (int i = 0; i < producer_num; i++)
        producers.emplace_back(std::make_unique<Producer>(&queue, production_num));
    for (int i = 0; i < consumer_num; i++)
        consumers.emplace_back(std::make_unique<Consumer>(&queue));

    // dtor
    return 0;
}
