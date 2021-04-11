#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <thread>
#include <memory>
#include <functional>





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

    Task(const Task& other)
    {
        printf("Task::copy constructor\n");
        this->_id = other._id;
    }

    Task& operator = (const Task& other)
    {
        printf("Task::copy assign operator\n");
        if  (&other != this)
        {
            this->~Task();
            new (this) Task(other);
        }
    }

    Task(Task&& other)
    {
        printf("Task::move constructor\n");
        this->_id = other._id;
        other._id = 0;
    }

    Task& operator = (Task&& other)
    {
        printf("Task::move assign operator\n");
        if  (&other != this)
        {
            this->~Task();
            new (this) Task(std::move(other));
        }
    }

    void print() const
    {
        printf("Task::print, id: %d\n", _id);
    }

};



typedef std::function<void(void)> func_t;




int main()
{
    printf("hello world\n");

    printf("1\n");
    {

        Task task(1);

        [task]() {

            // task._id = 2; // failed , task is const
            task.print();
        }();
        
    }
    printf("\n");


    printf("2\n");
    {

        Task task(1);

        [task]() mutable{

            task._id = 2;
            task.print();
        }();
        
    }
    printf("\n");

    printf("3\n");
    {

        Task task(1);

        [&task]() {

            task._id = 2; // failed , task is const
            task.print();
        }();
        
    }
    printf("\n");

    printf("4\n");
    {

        Task task(1);

        [task = std::move(task)]() mutable {

            task._id = 2; // failed , task is const
            task.print();
        }();
        
    }
    printf("\n");

    return 0;
}
