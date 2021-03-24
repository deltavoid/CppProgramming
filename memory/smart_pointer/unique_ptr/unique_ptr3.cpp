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

    Task(int id)
      : _id(id)
    {
        printf("Task::Task, id: %d\n", _id);
    }

    ~Task()
    {
        printf("Task::~Task, id: %d\n", _id);
    }

    void print()
    {
        printf("Task::print, id: %d\n", _id);
    }

};

void display_addr(const char* name, void* addr)
{
    printf("%s addr: %lx\n", name, addr);
}


std::unique_ptr<Task> set_task(std::unique_ptr<Task> task, int id)
{
    display_addr("set_task", task.get());
    printf("\n");
    task->_id = id;

    // return task;
    return std::move(task);
}


int main()
{

    printf("1\n");
    {
        std::unique_ptr<Task> uptr1(new Task(1));
        display_addr("uptr1", uptr1.get());
        printf("\n");

        std::unique_ptr<Task> uptr2(std::move(uptr1));
        display_addr("uptr1", uptr1.get());
        display_addr("uptr2", uptr2.get());
        printf("\n");

        uptr1.reset(uptr2.release());
        display_addr("uptr1", uptr1.get());
        display_addr("uptr2", uptr2.get());
        printf("\n");
        
    }
    printf("\n");

    printf("2\n");
    {
        std::unique_ptr<Task> uptr3 = std::make_unique<Task>(3);
        display_addr("uptr3", uptr3.get());
        printf("\n");

        std::unique_ptr<Task> uptr4 = std::move(uptr3);
        display_addr("uptr3", uptr3.get());
        display_addr("uptr4", uptr4.get());
        printf("\n");

        uptr3 = set_task(std::move(uptr4), 4);
        display_addr("uptr3", uptr3.get());
        display_addr("uptr4", uptr4.get());
        printf("\n");

    }

    return 0;
}