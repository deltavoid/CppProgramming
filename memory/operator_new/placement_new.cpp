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






int main()
{

    {
        Task* task = (Task*)malloc(sizeof(Task));

        new (task) Task(1);
        printf("\n");

        task->~Task();

        free(task);
    }
    printf("\n");


    return 0;
}