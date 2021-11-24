#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>


#include <thread>
#include <memory>



class Task {
public:
    int _id;

    Task()
    {
        printf("Task::Task\n");
    }

    ~Task()
    {
        printf("Task::~Task\n");
    }

};



int main()
{
    std::shared_ptr<Task> sptr = std::make_shared<Task>();

    std::weak_ptr<Task> wptr1(sptr);

    std::weak_ptr<Task> wtpr2 = sptr;

    std::weak_ptr<Task> wtpr3(wptr1);

    printf("sptr count: %d\n", sptr.use_count());
    printf("wptr1 count: %d\n", wptr1.use_count());
    printf("\n");


    if  (std::shared_ptr<Task> sptr1 = wptr1.lock())
    {
        printf("sptr1 count: %d\n", sptr1.use_count());
        printf("wptr1 count: %d\n", wptr1.use_count());
        printf("\n");
    }

    sptr.reset();

    if  (wptr1.expired())
    {
        printf("wptr1 is expired\n");
    }


    return 0;
}