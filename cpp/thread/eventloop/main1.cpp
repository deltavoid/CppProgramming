#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <thread>
#include <memory>
#include <vector>
#include <random>

#include <unistd.h>
#include <sys/file.h>
#include <sys/fcntl.h>


class Worker
{
public:    
    std::unique_ptr<std::thread> thread;
    volatile bool running;
    int id;


    Worker(int id)
        : id(id)
    {
        running = true;
        thread = std::make_unique<std::thread>([this]() { this->run(); });
    }

    ~Worker()
    {
        running = false;
        thread->join();
    }

    void run()
    {
        printf("Worker::run, id: %d\n", id);

        std::mt19937 rand(time(NULL) + id);

        while (running)
        {
            printf("id: %d, rand: %lld\n", id, rand());
            
            sleep(1);
        }
    }
};


int main(int argc, char **argv)
{
    int thread_num = 1;
    if (argc > 1)
        if (sscanf(argv[1], "%d", &thread_num) < 0)
        {   perror("bad thread_num");
            return -1;
        }

    int sleep_time = 10;
    if (argc > 2)
        if (sscanf(argv[2], "%d", &sleep_time) < 0)
        {   perror("bad sleep_time");
            return -1;
        }

    std::vector<std::unique_ptr<Worker>> workers(thread_num);
    for (int i = 0; i < workers.size(); i++)
        workers[i] = std::make_unique<Worker>(i);

    sleep(sleep_time);



    return 0;
}