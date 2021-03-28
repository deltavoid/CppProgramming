#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>


#include <thread>
#include <memory>
#include <exception>



// void* operator new(size_t size)
// {   
//     // if  (void* mem = malloc(size))
//     //     return mem;
//     // else
//     //     throw std::bad_alloc();
// }

void* operator new(size_t size)
{
    printf("overloaded global operator new begin\n");

    void* mem = malloc(size);
    if  (!mem)
        throw std::bad_alloc();

    printf("overloaded global operator new end\n");
    return mem;
}

void operator delete(void* mem)
{
    printf("overloaded global operator delete begin\n");    

    free(mem);

    printf("overloaded global operator delete end\n");
}




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
    printf("1\n");
    {
        Task task(1);
    }
    printf("\n");


    printf("2\n");
    {
        Task* task = new Task(2);
        printf("\n");

        delete task;
    }
    printf("\n");


    return 0;
}